# -*- coding:utf-8-unix -*-
TARGET    = bin/calc
LIBTARGET =

CFLAGS   += -std=c11 -I ../../include -I .
CXXFLAGS += 
LDFLAGS  += -L ../../lib
LDLIBS   += -lcparsec2

TEST_COMMAND = ./tests.sh

$(TARGET): ../../lib/libcparsec2.a

../../lib/libcparsec2.a: depend

depend:
	$(MAKE) lib -C ../..
