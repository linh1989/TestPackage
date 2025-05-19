AUTOMAKE_OPTIONS = subdir-objects

lib_LTLIBRARIES = libmodbus_service.la
bin_PROGRAMS 	= modbus_service

LIBS = -lpthread -levent -lprotobuf-c -lrt -lcommon -lm -levent_pthreads libmodbus

libmodbus_service_la_SOURCES = modbus_service_client.c \
                        protobuf-c/modbus_service.pb-c.c \
						utils/crc/crc32.c
if DEBUG
libmodbus_service_la_CFLAGS = -g3 -O0
else
libmodbus_service_la_CFLAGS = -O2 -DNDEBUG
endif
libmodbus_service_la_CFLAGS += -DLOG_NDEBUG=0
libmodbus_service_la_CFLAGS += -I./ \
						-I./protobuf-c/ \
						-I./utils/ \
						-I./utils/crc/
libmodbus_service_la_CFLAGS += -I $(top_srcdir)/../recipe-sysroot/usr/include/ccs
libmodbus_service_la_CFLAGS += -I $(top_srcdir)/../recipe-sysroot/usr/include/ccs/common 
modbus_service_SOURCES = modbus_service_main.c \
						  modbus_service.c \
						  modbus_service_impl.c \
						  modbus_service_intf.c \
						  modbus_service_rpc_adapter.c 
if DEBUG
modbus_service_CFLAGS = -g3 -O0 -Wall -Wextra -Werror
else
modbus_service_CFLAGS = -O2 -DNDEBUG -Wall -Wextra -Werror
endif
modbus_service_CFLAGS += -DLOG_NDEBUG=0
modbus_service_CFLAGS += -I./ \
						  -I./protobuf-c/ 
modbus_service_CFLAGS += -I $(top_srcdir)/../recipe-sysroot/usr/include/ccs
modbus_service_CFLAGS += -I $(top_srcdir)/../recipe-sysroot/usr/include/ccs/common 
modbus_service_LDADD = -lmodbus_service
modbus_service_LDFLAGS = -L./ $(LIBS) -L$(top_srcdir)/../build/src/.libs
