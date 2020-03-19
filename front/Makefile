CC = gcc
CFLAGS =`pkg-config --cflags glib-2.0` -I. -g -Wall -D_GNU_SOURCE=1 -std=c17 --ms-extensions
LIBS = `pkg-config --libs glib-2.0`

SRC_FILES=lang.c analysis.c icg.c front.c

LEXER_GEN_SRC=parser.l
PARSER_GEN_SRC=parser.y
LEXER=lex.yy.c
PARSER=parser.tab.c

all: compiler

test: compiler interp.py test.py
	python3 test.py

compiler: $(SRC_FILES) $(LEXER_GEN_SRC) $(PARSER_GEN_SRC)
	bison -vd $(PARSER_GEN_SRC)
	flex -o  $(LEXER) $(LEXER_GEN_SRC)
	$(CC) $(CFLAGS) $(SRC_FILES) $(LEXER) $(PARSER) $(LIBS) -o $@

clean:
	rm -f $(PARSER)
	rm -f $(LEXER)
	rm -f parser.tab.*
	rm -f compiler
