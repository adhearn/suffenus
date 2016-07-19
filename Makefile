CC = gcc
CFLAGS = -g -Wall
LIBS =
ifeq ($(shell uname -s),Darwin)
LIBS += -ll
else
LIBS += -lfl
endif

SRC_FILES=list.c compiler.c lang.c
INC_DIRS=-I.

LEXER_GEN_SRC=parser.l
PARSER_GEN_SRC=parser.y
LEXER=lex.yy.c
PARSER=parser.tab.c

all: compiler

compiler: $(SRC_FILES) $(LEXER_GEN_SRC) $(PARSER_GEN_SRC)
	bison -vd $(PARSER_GEN_SRC)
	flex -o  $(LEXER) $(LEXER_GEN_SRC)
	$(CC) $(CFLAGS) $(INC_DIRS) $(SRC_FILES) $(LEXER) $(PARSER) $(LIBS) -o $@

clean:
	rm -f $(PARSER)
	rm -f parser.tab.*
	rm -f parser.output
