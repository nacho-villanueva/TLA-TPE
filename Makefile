compile:
	yacc -d yacc.y;
	flex grammar.l;
	gcc -o a.out lex.yy.c y.tab.c -ly;

debug:
	yacc -d yacc.y;
	flex -d grammar.l;
	gcc -o a.out lex.yy.c y.tab.c -ly;

clean:
	rm -f lex.yy.c
	rm -f y.tab.c
	rm -f y.tab.h
	rm -f a.out

	