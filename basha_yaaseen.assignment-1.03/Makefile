assignment3: priorityQueue.o map.o worldGenerator.o
	gcc priorityQueue.o map.o worldGenerator.o -o assignment3

worldGenerator.o: worldGenerator.c worldGenerator.h
	gcc -Werror -g worldGenerator.c -c

map.o: map.c worldGenerator.h
	gcc -Werror -g map.c -c

priorityQueue.o: priorityQueue.c priorityQueue.h
	gcc -Werror -g priorityQueue.c -c

clean:
	rm -f *.o assignment3 *~
