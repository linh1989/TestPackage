
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>

#include "modbus_service_if.h"

static bool is_service_connected = false;

static const char* event_name[MODBUS_EVENT_ALL] = {
    "MODBUS_EVENT_EMETER_OUTPUT",
    "MODBUS_EVENT_ISO_STATUS",
    "MODBUS_EVENT_NODE_COMM_LOST",
    "MODBUS_EVENT_GET_EMETER_VERSION",
    "MODBUS_EVENT_GET_ISOCHA_VERSION",
    "MODBUS_EVENT_GET_EMETER_STATUS",
    "MODBUS_EVENT_GET_ISOCHA_STATUS",
    "MODBUS_EVENT_SET_SYSTEM_REBOOT",
    "MODBUS_EVENT_SET_ISO_MEASURE_STATE"
};

static void connect_callback(bool is_connected, void* context)
{
    char* app_name = (char*)context;
    is_service_connected = is_connected;
    printf("%s service %sconnected\n", app_name, is_service_connected ? "" : "dis");
}

static void event_callback(modbus_event_t event, void* data, void* context)
{
    assert(event < MODBUS_EVENT_ALL);

    char* app_name = (char*)context;
    printf("%s event received: %s\n", app_name, event_name[event]);

    switch (event) {
        case MODBUS_EVENT_SET_ISO_MEASURE_STATE:
        {
            modbus_iso_status_t* isolation_status = (modbus_iso_status_t*)data;
            printf("Isolation status: %d\n");
            break;
        }

        case MODBUS_EVENT_GET_EMETER_STATUS:
        {
            uint32_t* emeter_error = (uint32_t*)data;
            printf("Emeter error code: %d\n", *emeter_error);
            break;
        }

        case MODBUS_EVENT_GET_ISOCHA_STATUS:
        {
            uint32_t* isocha_error = (uint32_t*)data;
            printf("Isocha error code: %d\n", *isocha_error);
            break;
        }

        case MODBUS_RESPONSE_EMETER_OUTPUT:
        {
            modbus_meter_output_t* emeter_output = (modbus_meter_output_t*)data;
            printf("emeter_output m_id: %d\n", emeter_output->m_id);
            printf("emeter_output voltage: %d\n", emeter_output->voltage);
            printf("emeter_output current: %d\n", emeter_output->current);
            printf("emeter_output power: %d\n", emeter_output->power);
            break;
        }
        
        default:
            printf("Unknown event %d\n", event);
            break;
    }
}

int main(int argc, char* argv[])
{
    printf("Modbus client app\n");

    if (argc == 2) {
        int32_t event = atoi(argv[1]);
        if ((event >= MODBUS_EVENT_EMETER_OUTPUT) && (event <= MODBUS_EVENT_SET_ISO_MEASURE_STATE)) {
            printf("Event to register: %d\n", event);
            modbus_service_init(connect_callback, argv[0]);
                        
            printf("Waiting service to connect\n");
            do {
                sleep(1);
            }
            while (!is_service_connected);

            modbus_service_register_event(event, event_callback, argv[0]);
            
            while (true) {
                sleep(10);
            }

            modbus_service_free();

            return 0;
        }
    }

    printf("Register to an event from modbus service\n");
    printf("Usage: %s <event>\n", argv[0]);
    printf("       <event>:\n");
    for (uint8_t i = 0; i < MODBUS_EVENT_ALL; i++) {
        printf("       %2d -> %s\n", i, event_name[i]);
    }

    return -1;
}
