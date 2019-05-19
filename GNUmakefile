# -*- coding:utf-8-unix -*-

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

CFLAGS = -std=c11 -Wall -pedantic-errors

cparsec2: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJS): cparsec2.h

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

test: cparsec2
	./cparsec2

clean:
	rm -rf cparsec2 *.o *~ tmp*
