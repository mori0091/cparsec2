# -*- coding:utf-8-unix -*-
.PHONY: all build clean lib test

TARGET = bin/cparsec2
TARGET_LIB = lib/cparsec2.a

SRCS = $(wildcard src/*.c)
OBJS = $(patsubst src/%.c, obj/%.o, $(SRCS))
DEPS = $(patsubst %.o, %.d, $(OBJS))
COVS = $(patsubst %.o, %.gcda, $(OBJS)) $(patsubst %.o, %.gcno, $(OBJS))
LIBOBJS = $(filter-out %/main.o, $(OBJS))

CFLAGS ?=
CFLAGS += -MMD -MD
CFLAGS += -pedantic-errors -Wall -Wpedantic -Wextra
CFLAGS += -Winit-self -Wno-missing-field-initializers

CFLAGS += -std=c11 -I include

all: build lib

build: $(TARGET)

lib: $(TARGET_LIB)

test: $(TARGET)
	./$(TARGET)

clean:
	@rm -f $(TARGET) $(TARGET_LIB) $(OBJS) $(DEPS) $(COVS) *~
	@rm -df obj bin lib

$(TARGET): $(OBJS)
	$(info [LD]    Build   : $@	[$(notdir $(CURDIR))])
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(TARGET_LIB): $(LIBOBJS)
	$(info [AR]    Build   : $@	[$(notdir $(CURDIR))])
	@mkdir -p $(dir $@)
	@$(AR) cr $@ $^

obj/%.o: src/%.c
	$(info [C]     Compile : $@ ($<))
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c -o $@ $<

-include $(DEPS)
