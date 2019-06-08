# -*- coding:utf-8-unix -*-
.PHONY: all build clean lib test

all: build lib

include .project.mk

DEFAULT_CFLAGS ?=
DEFAULT_CFLAGS += -MMD
DEFAULT_CFLAGS += -pedantic-errors -Wall -Wpedantic -Wextra
DEFAULT_CFLAGS += -Winit-self -Wno-missing-field-initializers

CFLAGS   ?=
CFLAGS   += $(DEFAULT_CFLAGS)
CXXFLAGS ?=
CXXFLAGS += $(DEFAULT_CFLAGS)

TESTFLAGS ?=

SRCS_C = $(wildcard src/*.c)
OBJS_C = $(patsubst src/%.c, obj/%.o, $(SRCS_C))

SRCS_CPP = $(wildcard src/*.cpp)
SRCS_CXX = $(wildcard src/*.cxx)
OBJS_CPP = $(patsubst src/%.cpp, obj/%.o, $(SRCS_CPP))
OBJS_CXX = $(patsubst src/%.cxx, obj/%.o, $(SRCS_CXX))

SRCS = $(SRCS_C) $(SRCS_CPP) $(SRCS_CXX)
OBJS = $(OBJS_C) $(OBJS_CPP) $(OBJS_CXX)
DEPS = $(patsubst %.o, %.d, $(OBJS))
COVS = $(patsubst %.o, %.gcda, $(OBJS)) $(patsubst %.o, %.gcno, $(OBJS))
LIBOBJS = $(filter-out %/main.o, $(OBJS))

TEST_COMMAND ?= ./$(TARGET) $(TESTFLAGS)

build: $(TARGET)

lib: $(LIBTARGET)

test: $(TARGET)
	$(info [TEST] $(TEST_COMMAND))
	@$(TEST_COMMAND)

clean:
	@rm -f $(TARGET) $(TARGET).exe $(LIBTARGET) $(OBJS) $(DEPS) $(COVS) *~
	@rm -f *.gcov
	@rm -df obj bin lib

cov: CFLAGS   += -coverage
cov: CXXFLAGS += -coverage
cov: LDFLAGS  += -coverage
cov: all test
	@gcov -abcpru -o obj $(SRCS)

$(TARGET): $(OBJS)
	$(info [LD]    Build   : $@	[$(notdir $(CURDIR))])
	@mkdir -p $(dir $@)
	@$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

$(LIBTARGET): $(LIBOBJS)
	$(info [AR]    Build   : $@	[$(notdir $(CURDIR))])
	@mkdir -p $(dir $@)
	@$(AR) cr $@ $^

obj/%.o: src/%.c
	$(info [C]     Compile : $@ ($<))
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c -o $@ $<

obj/%.o: src/%.cpp
	$(info [C++]   Compile : $@ ($<))
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

obj/%.o: src/%.cxx
	$(info [C++]   Compile : $@ ($<))
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

-include $(DEPS)
