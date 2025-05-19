AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = subdir-objects

bin_PROGRAMS = modbus_service_sub modbus_service_pub

LIBS = -lpthread -levent -lprotobuf-c -lrt

modbus_service_sub_SOURCES = modbus_service_sub.c
modbus_service_sub_CFLAGS = -I$(top_srcdir)/src \
                     -I./ \
                     -I$(top_srcdir)/src/protobuf-c \
                     -I./utils
modbus_service_sub_LDADD = -lmodbus_service
modbus_service_sub_LDFLAGS = -L./ $(LIBS) -L$(top_srcdir)/../build/src/.libs

modbus_service_pub_SOURCES = modbus_service_pub.c
modbus_service_pub_CFLAGS = -I$(top_srcdir)/src \
                     -I./ \
                     -I$(top_srcdir)/src/protobuf-c \
                     -I./utils
modbus_service_pub_LDADD = -lmodbus_service
cmodbus_service_pub_LDFLAGS = -L./ $(LIBS) -L$(top_srcdir)/../build/src/.libs
