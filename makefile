all: treesearch treeload

treesearch: treesearch.o mylib/libmylib.a
	gcc -o treesearch treesearch.o -Lmylib -lmylib

treeload: treeload.o mylib/libmylib.a
	gcc -o treeload treeload.o -Lmylib -lmylib


treesearch.o: treesearch.c mylib/libmylib.a
	gcc -c treesearch.c

treeload.o: treeload.c mylib/libmylib.a
	gcc -c treeload.c
			
	