/****************************************************
 *  modbus_service.c
 *  Created on: 03-May-2025 10:41:05
 *  Implementation of the Class modbus_service
 *  Original author: linhnm
 ****************************************************/

#define LOG_TAG "modbus_service"

#include "modbus_service.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>

#include "logger.h"
#include "modbus_service_client.h"
#include "modbus_service_intf.h"
#include "modbus_service_impl.h"
#include "modbus_service_rpc_adapter.h"

#define EV_CONN_LISTENER_BACKLOG -1

#define RESP_LEN_MAX 64

#define REC_ALERT_SIG_NUM 8

static void* read_cb(void *ctx);
static void on_rtu_rx_cb(modbus_response_t response, void *data, void *context);
static void send_response(int client_fd, modbus_response_t response, uint32_t len, uint8_t *data);

modbus_err_t modbus_service_run(const char *service)
{
	FUNC_ENTRY();

	memset(rec_error_state,    0, sizeof(rec_error_state));

	const modbus_impl_handle_t *rtu_hdl_ptr = modbus_impl_get_handle();
	rtu_hdl_ptr->run();

	SLOGI("%s", "Start listening");
	int sock_fd =  socket(AF_INET, SOCK_STREAM, 0);
	if (sock_fd < 0)
	{
		SLOGE("%s", "Error opening socket");
		exit(-1);
	}

	int opt = 1;
	if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        SLOGE("%s", "Error setting socket option");
        exit(EXIT_FAILURE);
    }

	struct sockaddr_in addr;
	addr.sin_family		 = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port		 = htons(SOCKET_PORT);

	if (bind(sock_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
	{
        SLOGE("%s", "Error binding socket");
        exit(-1);
    }

	int ret = listen(sock_fd, 5);
	if (ret < 0) {
		SLOGE("%s", "Error listenning");
		exit(-1);
	}

	struct sockaddr_in addr_in;
	uint32_t addr_len = sizeof(addr_in);
	while (true)
	{
		int client_fd = accept(sock_fd, (struct sockaddr*)&addr_in, &addr_len);
		if (client_fd < 0)
		{
			usleep(500000);
			continue;
		}

		connection_data_t* conn_data = (connection_data_t *)malloc(sizeof(connection_data_t));
		conn_data->client_fd = client_fd;

		pthread_attr_t tattr;
        pthread_attr_init(&tattr);
		size_t size = 0x20000;//128K
    	pthread_attr_setstacksize(&tattr, size);

		pthread_t client_thread;
		pthread_create(&client_thread, &tattr, read_cb, conn_data);
	}

	FUNC_EXIT();
	return MODBUS_ERR_NONE;
}

static void* read_cb(void *ctx)
{
	FUNC_ENTRY();

	connection_data_t*    conn_data = (connection_data_t*)ctx;
	modbus_impl_subject* subject   = modbus_impl_get_subject();

	conn_data->receiver  = modbus_impl_create_receiver(conn_data, on_rtu_rx_cb);
	subject->register_receiver(conn_data->receiver);

	uint8_t  buffer[4096];
	while (true)
	{
		int read_len = read(conn_data->client_fd, buffer, sizeof(buffer));
		if (read_len <= 0) {
			SLOGE("%s", "Client disconnected (read failed)");
			subject->unregister_receiver(conn_data->receiver);
			free(conn_data->receiver);
			free(conn_data);
			return NULL;
		}

		uint8_t* buff_ptr = buffer;
		size_t len = (size_t)read_len;

		while (len >= sizeof(uint64_t))
		{
			int32_t reqhdr_data_len = rpc_read_pack_len(buff_ptr, len);
			if (reqhdr_data_len < 0)
			{
				SLOGE("Error reading reqhdr len %d\n", reqhdr_data_len);
				break;
			}

			if (len >= (reqhdr_data_len + sizeof(uint64_t)))
			{
				Rpcproto__ReqHeader *reqhdr = rpc_read_req_data(buff_ptr, reqhdr_data_len);
				if (!reqhdr) {
					break;
				}
				
				Rpcproto__RspHeader rsphdr = RPCPROTO__RSP_HEADER__INIT;
				modbus_err_t ret = rpc_invoke_call(reqhdr, &rsphdr, conn_data);
				if (ret != MODBUS_ERR_NONE)
				{
					SLOGI("%s", "rpc_invoke_call failed");
				}

				// Write resp directly in the case of synchronous function
				if (reqhdr->issync)
				{
					uint8_t *outbuf = NULL;
					ret = rpc_write_reply(&rsphdr, &outbuf);
					if (ret <= 0)
					{
						SLOGI("%s %d", "rpc_write_reply failed ret = : ", ret);
					}

					if (write(conn_data->client_fd, outbuf, ret) < 0) {
						SLOGE("%s", "Client disconnected (write failed)");
						subject->unregister_receiver(conn_data->receiver);
						free(conn_data->receiver);
						free(conn_data);
						return NULL;
					}

					free(rsphdr.result.data);
				}

				rpc_clear_req(reqhdr);

				int32_t req_len = reqhdr_data_len + sizeof(uint64_t);
				len -= req_len;
				buff_ptr += req_len;
			}
			else
			{
				break;
			}
		}
	}

	FUNC_EXIT();
}

static void on_rtu_rx_cb(modbus_response_t response, void *data, void *context)
{
	FUNC_ENTRY();

	size_t rsp_len = 0;
	uint8_t buffer[RESP_LEN_MAX];
	memset(buffer, 0, sizeof(buffer));

	SLOGI("On response callback %d\n", response);

	switch (response)
	{
	case MODBUS_RESPONSE_EMETER_OUTPUT:
	{
		uint32_t m_id_data = 0;
		uint32_t volt_data;
		uint32_t curr_data;
		uint32_t power_data;

		ModbusService__EmeterOutput rsp = MODBUS_SERVICE__EMETER_OUTPUT__INIT;
		rsp.m_id = (uint32_t)(m_id_data);
		rsp.voltage = (uint32_t)(volt_data * 1000);
		rsp.current = (uint32_t)(curr_data * 1000);
		rsp.power_data = (uint32_t)(power_data * 1000000);
		rsp_len = modbus_service__emeter_output__pack(&rsp, buffer);
		break;
	}

	case MODBUS_RESPONSE_ISO_STATUS:
	{
		ModbusService__IsochaIsoStatus rsp = MODBUS_SERVICE__ISOCHA_ISO_STATUS__INIT;
		rsp.status = 0;
		rsp_len = modbus_service__isocha_iso_status__pack(&rsp, buffer);
		break;
	}

	case MODBUS_RESPONSE_GET_EMETER_VERSION:
	{
		uint32_t major = 0;
		uint32_t minor = 0;
		uint32_t subminor = 0;

		ModbusService__Version rsp = MODBUS_SERVICE__VERSION__INIT;
		rsp.major = (uint32_t)(major);
		rsp.minor = (uint32_t)(minor);
		rsp.subminor = (uint32_t)(subminor);

		rsp_len = modbus_service__version__pack(&rsp, buffer);
		break;
	}

	case MODBUS_RESPONSE_GET_ISOCHA_VERSION:
	{
		uint32_t major = 0;
		uint32_t minor = 0;
		uint32_t subminor = 0;

		ModbusService__Version rsp = MODBUS_SERVICE__VERSION__INIT;
		rsp.major = (uint32_t)(major);
		rsp.minor = (uint32_t)(minor);
		rsp.subminor = (uint32_t)(subminor);

		rsp_len = modbus_service__version__pack(&rsp, buffer);
		break;
	}

	case MODBUS_RESPONSE_GET_EMETER_STATUS:
	{
		uint32_t tmp_error_code = 0;

		ModbusService__EmeterError rsp = MODBUS_SERVICE__EMETER_ERROR__INIT;
		rsp.error_code = (uint32_t)(tmp_error_code);

		rsp_len = modbus_service__emeter_error__pack(&rsp, buffer);
		break;
	}

	case MODBUS_RESPONSE_GET_ISOCHA_STATUS:
	{
		uint32_t tmp_error_code = 0;

		ModbusService__IsochaError rsp = MODBUS_SERVICE__ISOCHA_ERROR__INIT;
		rsp.error_code = (uint32_t)(tmp_error_code);

		rsp_len = modbus_service__isocha_error__pack(&rsp, buffer);
		break;
	}

	default:
		SLOGI("%s", "Get new unexpected event\n");
		break;
	}

	if (rsp_len)
	{
		connection_data_t *conn_data = (connection_data_t *)context;
		send_response(conn_data->client_fd, response, rsp_len, buffer);
	}

	FUNC_EXIT();
}

void send_response(int client_fd, modbus_response_t response, uint32_t len, uint8_t *data)
{
	Rpcproto__RspHeader rsphdr = RPCPROTO__RSP_HEADER__INIT;
	rsphdr.id = response;
	rsphdr.result.data = data;
	rsphdr.result.len = len;

	uint8_t *outbuf = NULL;
	int32_t rsp_len = rpc_write_reply(&rsphdr, &outbuf);
	if (rsp_len < 0)
	{
		SLOGE("%s", "rpc_write_reply failed");
	}

	write(client_fd, outbuf, (size_t)rsp_len);
	free(outbuf);
}
