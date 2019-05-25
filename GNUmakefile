# -*- coding:utf-8-unix -*-

TARGET = bin/cparsec2

SRCS = $(wildcard src/*.c)
OBJS = $(patsubst src/%.c, obj/%.o, $(SRCS))
DEPS = $(patsubst %.o, %.d, $(OBJS))
COVS = $(patsubst %.o, %.gcda %.gcno, $(OBJS))

CFLAGS ?=
CFLAGS += -MMD -MD
CFLAGS += -pedantic-errors -Wall -Wpedantic -Wextra
CFLAGS += -Winit-self -Wno-missing-field-initializers

CFLAGS += -std=c11 -I include

$(TARGET): $(OBJS)
	$(info [LD]    Build   : $@	[$(notdir $(CURDIR))])
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

obj/%.o: src/%.c
	$(info [C]     Compile : $@ ($<))
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c -o $@ $<

test: $(TARGET)
	./$(TARGET)

clean:
	@rm -f $(TARGET) $(OBJS) $(DEPS) $(COVS) *~
	@rm -df obj bin

-include $(DEPS)
