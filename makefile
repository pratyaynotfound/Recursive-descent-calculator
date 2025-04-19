objfiles = recursiveDescent.o lex.yy.o

a.out: $(objfiles)
	cc $(objfiles)

recursiveDescent.o: recursiveDescent.c y.tab.h
	cc -c -Wall recursiveDescent.c

lex.yy.o: lex.yy.c
	cc -c lex.yy.c

lex.yy.c: lex.l y.tab.h
	flex lex.l
	
clean:
	rm a.out lex.yy.c $(objfiles)