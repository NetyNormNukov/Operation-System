# Makefile for Task1 with library
# CC = gcc

# main: main.o libfun.a
# 	$(CC) $(CFLAGS) -o main task2.o -L. -lfun -lm
# main.o: main.c
# 	$(CC) $(CFLAGS) -c main.c
# libfun.a: point.o
# 	ar r libfun.a point.o
# 	ranlib libfun.a
# point.o: point.c point.h
# 	$(CC) $(CFLAGS) -c point.c 
# clean:
# 	rm -rf *.o *.a main
all: mainTarget
		
mainTarget: point.o main.o
		gcc main.o point.o -o task1 -lm
main.o: main.c point.h
		gcc -c main.c 
point.o: point.c point.h
		gcc -c point.c
clean: 
		rm -rf *.o mainTarget
run:
		./task1

