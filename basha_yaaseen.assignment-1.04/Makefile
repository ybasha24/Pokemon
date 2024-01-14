assignment4: characterQueue.o  priorityQueue.o  map.o  worldGenerator.o
	gcc characterQueue.o  priorityQueue.o map.o worldGenerator.o -o assignment4

worldGenerator.o: worldGenerator.c worldGenerator.h
	gcc -Werror -g worldGenerator.c -c

map.o: map.c worldGenerator.h
	gcc -Werror -g map.c -c

characterQueue.o: characterQueue.c characterQueue.h
	gcc -Werror -g characterQueue.c -c

priorityQueue.o: priorityQueue.c priorityQueue.h
	gcc -Werror -g priorityQueue.c -c

clean:
	rm -f *.o assignment3 *~
