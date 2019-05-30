# -*- coding:utf-8-unix -*-
TARGET    = tests
LIBTARGET =

CFLAGS   += -std=c11 -I ../include -I .
CXXFLAGS += -std=c++17 -I ../include -I . -I ../thirdparty/catch2
LDFLAGS  += -L ../lib
LDLIBS   += -lcparsec2

# includes successful tests in output ('make test')
# TESTFLAGS += -s

# show invisibles (tabs, newlines)
TESTFLAGS += -i

$(TARGET): ../lib/libcparsec2.a

../lib/libcparsec2.a: depend

depend:
	$(MAKE) lib -C ..
