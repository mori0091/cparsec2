# -*- coding:utf-8-unix -*-
TARGET    = bin/calcc

CFLAGS   += -std=c11 -I ../../include -I include -I .
LDFLAGS  += -L ../../lib
LDLIBS   += -lcparsec2

TEST_COMMAND = ./tests.sh

$(TARGET): ../../lib/libcparsec2.a

../../lib/libcparsec2.a: depend

depend:
	$(MAKE) lib -C ../..
