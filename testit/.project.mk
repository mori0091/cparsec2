# -*- coding: utf-8-unix -*-
TARGET = AllTests

CFLAGS   += -D_POSIX_C_SOURCE
CFLAGS   += -std=c11 -I include -I ../include
LDFLAGS  += -L ../lib
LDLIBS   += -lcparsec2

$(TARGET): ../lib/libcparsec2.a

../lib/libcparsec2.a: depend

depend:
	$(MAKE) lib -C ..
