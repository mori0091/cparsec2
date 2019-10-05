# -*- coding:utf-8-unix -*-
.PHONY: all build clean cov gcov lib test

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

SRCDIR ?= src
OBJDIR ?= obj

SRCS = $(shell find $(SRCDIR) -type f \
                    -name '*.c'   -o  \
                    -name '*.cpp' -o  \
                    -name '*.cxx')

OBJS = $(patsubst $(SRCDIR)/%.c,   $(OBJDIR)/%.o, \
       $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, \
       $(patsubst $(SRCDIR)/%.cxx, $(OBJDIR)/%.o, \
       $(SRCS))))

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
	@rm -f *.gcov $(OBJDIR)/*.gcov
	@rm -rf $(OBJDIR)
	@rm -df bin lib

# `make cov`: build and test for coverage test
# To make coverage report:
# 1. do `make cov` for ALL (sub)project, then
# 2. do `gcov [options] -o obj src/*` for each (sub)project,
#    or do `bash <(curl -s https://codecov.io/bash)`
cov: CFLAGS   += -coverage
cov: CXXFLAGS += -coverage
cov: LDFLAGS  += -coverage
cov: all test

# 'make gcov': generates coverage report
gcov:
	$(foreach d, $(sort $(dir $(SRCS))), \
		gcov -abcpu \
			-o $(patsubst $(SRCDIR)/%, $(OBJDIR)/%, $(d)) \
			$(filter $(d)%.c $(d)%.cpp $(d)%.cxx, $(SRCS)) ;)

$(TARGET): $(OBJS)
	$(info [LD]    Build   : $@	[$(notdir $(CURDIR))])
	@mkdir -p $(dir $@)
	@$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

$(LIBTARGET): $(LIBOBJS)
	$(info [AR]    Build   : $@	[$(notdir $(CURDIR))])
	@mkdir -p $(dir $@)
	@$(AR) cr $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(info [C]     Compile : $@ ($<))
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(info [C++]   Compile : $@ ($<))
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJDIR)/%.o: $(SRCDIR)/%.cxx
	$(info [C++]   Compile : $@ ($<))
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

-include $(DEPS)
