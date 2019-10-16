# -*- coding:utf-8-unix -*-
TARGET    = bin/sample

CFLAGS   += -std=c11 -I ../../include -I include -I .
LDFLAGS  += -L ../../lib
LDLIBS   += -lcparsec2

$(TARGET): ../../lib/libcparsec2.a

../../lib/libcparsec2.a: depend

depend:
	$(MAKE) lib -C ../..
