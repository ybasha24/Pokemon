assignment2: worldGenerator.o map.o
	gcc worldGenerator.o map.o -o assignment2

worldGenerator.o: worldGenerator.c worldGenerator.h
	gcc -Werror -g worldGenerator.c -c

map.o: map.c worldGenerator.h
	gcc -Werror -g map.c -c

clean:
	rm -f *.o assignment2 *~
