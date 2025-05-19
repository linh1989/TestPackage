#!/bin/sh

libtoolize --copy --force
aclocal ${OECORE_ACLOCAL_OPTS} &&\
autoheader &&\
autoconf &&\
automake --add-missing

#./configure ${CONFIGURE_FLAGS}
