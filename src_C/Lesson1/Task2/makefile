

mainTarget: ArrayList.o main.o
		gcc  main.o ArrayList.o -o completeTask -lm 
ArrayList.o: ArrayList.c 
		gcc -c ArrayList.c
main.o: main.c  ArrayList.c
		gcc -c main.c
clean: 
		rm -rf *.o mainTarget
start:
		./completeTask