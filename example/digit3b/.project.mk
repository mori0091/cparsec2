# -*- coding:utf-8-unix -*-
TARGET    = bin/digit3b
LIBTARGET =

CFLAGS   += -std=c11 -I ../../include -I .
CXXFLAGS += 
LDFLAGS  += -L ../../lib
LDLIBS   += -lcparsec2

$(TARGET): ../../lib/libcparsec2.a

../../lib/libcparsec2.a: depend

depend:
	$(MAKE) lib -C ../..
