libmylib.a:	load.o search.o parse.o
	ar -rs $@ load.o search.o parse.o
	
parse.o: parse.c mylib.h
	cc -c parse.c mylib.h

search.o: search.c mylib.h
	cc -c search.c	

load.o: load.c mylib.h
	cc -c load.c
		