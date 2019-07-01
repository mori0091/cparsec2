# -*- coding:utf-8-unix -*-
TARGET    = bin/cparsec2
LIBTARGET = lib/libcparsec2.a

CFLAGS   += -D_POSIX_C_SOURCE=200809L -D_FILE_OFFSET_BITS=64
CFLAGS   += -std=c11 -I include
LDFLAGS  +=
LDLIBS   +=
