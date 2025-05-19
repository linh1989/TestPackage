
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>

#include "modbus_service_if.h"

enum {
    MODBUS_REQUEST_REGISTER_EVENT = 0,
    MODBUS_REQUEST_UNREGISTER_EVENT,
    MODBUS_REQUEST_EMETER_OUTPUT,
    MODBUS_REQUEST_GET_EMETER_VERSION,
    MODBUS_REQUEST_GET_ISOCHA_VERSION,
    MODBUS_REQUEST_GET_EMETER_STATUS,
    MODBUS_REQUEST_GET_ISOCHA_STATUS,
    MODBUS_REQUEST_SET_SYSTEM_REBOOT,
    MODBUS_REQUEST_SET_ISO_MEASURE_STATE,
    MODBUS_REQUEST_MAX
};

#define INPUT_ARG_MIN               2
#define APP_NAME_OFFSET             0

static bool is_service_connected = false;

static char* req_name[MODBUS_REQUEST_MAX] = {
    "Get emeter output info",
    "Get emeter version info",
    "Get isocha version info",
    "Get emeter error code info",
    "Get isocha error code info",
    "Set isolation measurement state",
    "Set system reboot",
};

static uint8_t req_arg_count[MODBUS_REQ_MAX] = {
    3,      // MODBUS_REQUEST_EMETER_OUTPUT
    1,      // MODBUS_REQUEST_GET_EMETER_VERSION
    1,      // MODBUS_REQUEST_GET_ISOCHA_VERSION
    2,      // MODBUS_REQUEST_GET_EMETER_STATUS
    2,      // MODBUS_REQUEST_GET_ISOCHA_STATUS
    1,      // MODBUS_REQUEST_SET_SYSTEM_REBOOT
    0,      // MODBUS_REQUEST_SET_ISO_MEASURE_STATE
};

static void connect_callback(bool is_connected, void* context)
{
    char* app_name = (char*)context;
    is_service_connected = is_connected;
    printf("%s service %sconnected\n", app_name, is_service_connected ? "" : "dis");
}

static void event_callback(modbus_event_t event, void* data, void* context)
{

}

int main(int argc, char* argv[])
{
    printf("Modbus client app\n");

    if (argc >= INPUT_ARG_MIN) {
        uint8_t req_id = (uint8_t)atoi(argv[REQ_ID_OFFSET]);
        printf("Request Id: %d\n", req_id);
        
        if (req_id < MODBUS_REQ_MAX) {
            uint8_t arg_count = (uint8_t)(argc - INPUT_ARG_MIN);
            if (arg_count == req_arg_count[req_id]) {
                printf("Send %s request\n", req_name[req_id]);
                modbus_init(connect_callback, argv[APP_NAME_OFFSET]);

                printf("Waiting service to connect\n");
                do {
                    sleep(1);
                }
                while (!is_service_connected);

                modbus_register_event(MODBUS_EVENT_ALL, event_callback, argv[0]);

                modbus_err_t error = MODBUS_ERR_UNKNOWN;

                switch (req_id) {
                    case MODBUS_REQUEST_EMETER_OUTPUT:
                    {
                        modbus_meter_output_t emeter_info;
                        error = modbus_get_fan_info(&emeter_info);
                        if (error == MODBUS_ERR_NONE) {
                            printf("Emeter info:");
                            printf(" [m_id-%d]", emeter_info.m_id);
                            printf(" [Voltage-%d]", emeter_info.Voltage);
                            printf(" [Current-%d]", emeter_info.Current);
                            printf(" [Power-%d]", emeter_info.Power);
                            printf("\n");
                        }
                        break;
                    }

                    case MODBUS_REQUEST_GET_EMETER_VERSION:
                    {
                        modbus_version_t version_info;
                        error = modbus_get_emeter_version_infor(&version_info);
                        if (error == MODBUS_ERR_NONE) {
                            printf("Emeter Version info:");
                            printf(" [Major-%d]", version_info.major);
                            printf(" [Minor-%d]", version_info.minor);
                            printf(" [Subminor-%d]", version_info.subminor);
                            printf("\n");
                        }
                        break;
                    }

                    case MODBUS_REQUEST_GET_ISOCHA_VERSION:
                    {
                        modbus_version_t version_info;
                        error = modbus_get_isocha_version_infor(&version_info);
                        if (error == MODBUS_ERR_NONE) {
                            printf("Emeter Version info:");
                            printf(" [Major-%d]", version_info.major);
                            printf(" [Minor-%d]", version_info.minor);
                            printf(" [Subminor-%d]", version_info.subminor);
                            printf("\n");
                        }
                        break;
                    }

                    case MODBUS_REQUEST_GET_EMETER_STATUS:
                    {
                        uint32_t error_code_info;
                        error = modbus_get_emeter_error_code(&error_code_info);
                        if (error == MODBUS_ERR_NONE) {
                            printf("Emeter Error Code info:");
                            printf(" [error_code_info-%d]", error_code_info);
                            printf("\n");
                        }
                        break;
                    }

                    case MODBUS_REQUEST_GET_ISOCHA_STATUS:
                    {
                        uint32_t error_code_info;
                        error = modbus_get_isocha_error_code(&error_code_info);
                        if (error == MODBUS_ERR_NONE) {
                            printf("IsoCHA Error Code info:");
                            printf(" [error_code_info-%d]", error_code_info);
                            printf("\n");
                        }
                        break;
                    }

                    case MODBUS_REQUEST_SET_ISO_MEASURE_STATE:
                    {
                        bool is_start = true;
                        error = modbus_set_iso_measurement_state(is_start);
                        break;
                    }

                    case MODBUS_REQUEST_SET_SYSTEM_REBOOT:
                        printf("Set system reboot\n");
                        error = modbus_set_system_reboot();
                        break;

                    default:
                        error = MODBUS_ERR_INVALID_PARAM;
                        break;
                }

                printf("Send request %s\n", (error == MODBUS_ERR_NONE) ? "succeeded" : "failed");

                sleep(2);

                modbus_service_free();

                return 0;
            }
        }
    }

    printf("Send a request to modbus service\n");
    printf("Usage: %s <request> [data]\n", argv[APP_NAME_OFFSET]);
    printf("       <request>:\n");
    printf("       0  -> Get emeter output info \n");
    printf("       1  -> Get emeter version info\n");
    printf("       2  -> Get isocha version info\n");
    printf("       3  -> Get emeter error code info\n");
    printf("       4  -> Get isocha error code info\n");
    printf("       5  -> Set isolation measurement state: 1 - on / 0 - off\n");
    printf("       6  -> Set system reboot\n");
    printf("       <node_id>:\n");
    printf("       0x01 -> Emeter\n");
    printf("       0x03 -> IsoCHA\n");

    return -1;
}
