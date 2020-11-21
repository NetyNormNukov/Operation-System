MainTargen: environ.o func.o
	gcc -o environ func.o environ.o -lm
func.o: func.c func.h
	gcc -c func.c 
environ.o: func.c func.h environ.c
	gcc -c environ.c
run:
		./environ
clean: 
		rm -rf *.o MainTarget