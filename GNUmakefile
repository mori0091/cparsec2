# -*- coding:utf-8-unix -*-

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

cparsec2: $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

$(OBJS): cparsec2.h

test: cparsec2
	./cparsec2

clean:
	rm -rf cparsec2 *.o *~ tmp*
