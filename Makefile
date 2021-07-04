LIB_SOURCES=$(wildcard */*.*.c  */*.c *.c *.*.c)
LIB_SOURCES=
LANG_SOURCES=parser.c lexer.c main.c kernel.c
BISON=bison
FLEX=flex

et: clean bison flex build run compile

bison: parser.y
	$(BISON) -d parser.y -o parser.c

flex: lexer.l
	$(FLEX) -o lexer.c lexer.l

build:
	$(CC) $(LIB_SOURCES) $(LANG_SOURCES) -w -o et
	@rm -f parser.c
	@rm -f parser.h
	@rm -f lexer.c

clean:
	@rm -f parser.c
	@rm -f parser.h
	@rm -f lexer.c
	@rm -f ./et
	@rm -f temp.c
	@rm -f temp

run:
	./et input

compile:
	@gcc temp.c -o temp
	@./temp
	@#rm temp
