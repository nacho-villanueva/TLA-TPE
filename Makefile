YACC_IN = src/yacc.y
YACC_FLAGS = -Wall -Werror -d -Wno-empty-rule

LEX_IN = src/grammar.l
LEX_FLAGS =

CC = gcc
GCC_OUT = u3dc
GCC_FLAGS = -Iutils/ -Wall -Wno-unused-variable -g

all: build compile 
	
	

build:
	${YACC} ${YACC_FLAGS} ${YACC_IN};
	${LEX} ${LEX_FLAGS} ${LEX_IN};

lex_debug: LEX_FLAGS += -d
lex_debug: all

u3d_debug: GCC_FLAGS += -DDEBUG
u3d_debug: all 

debug: LEX_FLAGS +=
debug: GCC_FLAGS += -DDEBUG
debug: all


compile:
	${CC} ${GCC_FLAGS} -o ${GCC_OUT} src/utils/* src/nodes/*.c src/u3d.c lex.yy.c y.tab.c ;


clean:
	rm -f lex.yy.c
	rm -f y.tab.c
	rm -f y.tab.h
	rm -f ${GCC_OUT}
	rm -f u3dc.log

	
