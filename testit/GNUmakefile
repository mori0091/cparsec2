# -*- coding:utf-8-unix -*-
include ../GNUmakefile

TESTITC = sh ./testit.sh
TESTIT_DB_SRC = $(TARGET)_db.c
TESTIT_DB_OBJ = $(TESTIT_DB_SRC:.c=.o)

$(TARGET): $(TESTIT_DB_OBJ)

$(TESTIT_DB_SRC): $(OBJS)
	@mkdir -p $(dir $@)
	@$(TESTITC) $^ > $@

clean: clean_

clean_:
	@rm -f $(TESTIT_DB_SRC) $(TESTIT_DB_OBJ) *.d
