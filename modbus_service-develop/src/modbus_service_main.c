
#define LOG_TAG "modbus_service_main"

#include "logger.h"

#include "modbus_service.h"

int main(int argc, char **argv) {
    SLOG_INIT();

    if (modbus_service_run(argv[0]) != MODBUS_SERVICE_ERR_NONE) {
        SLOGE("%s", "Failed to start service");
        return -1;
    }

    SLOGE("%s", "Ops,something went wrong! Service stopped unintentionally");
    SLOG_DEINIT();

    return 0;
}
