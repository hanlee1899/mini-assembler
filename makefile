assem_compiler: main.o
	gcc -o assem_compiler main.o

main.o: main.c
	gcc -c -o main.o main.c
	
clean:
	rm *.o assem_compiler