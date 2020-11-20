YACC_IN = yacc.y
YACC_FLAGS = -Wall -Werror -d -Wno-empty-rule

LEX_IN = grammar.l
LEX_FLAGS =

CC = gcc
GCC_OUT = u3dc
GCC_FLAGS = -Iutils/ -ly -Wall -Wno-unused-variable

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
	${CC} -o ${GCC_OUT} utils/* nodes/*.c u3d.c lex.yy.c y.tab.c ${GCC_FLAGS};


clean:
	rm -f lex.yy.c
	rm -f y.tab.c
	rm -f y.tab.h
	rm -f ${GCC_OUT}

	