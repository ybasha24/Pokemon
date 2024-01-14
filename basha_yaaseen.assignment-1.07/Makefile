assignment7: characterQueue.o  priorityQueue.o  map.o  worldGenerator.o
	g++ -o assignment7 characterQueue.o  priorityQueue.o map.o worldGenerator.o -lncurses 

worldGenerator.o: worldGenerator.cpp worldGenerator.h parse.h
	g++ -c -I  -g worldGenerator.cpp

map.o: map.cpp worldGenerator.h
	g++ -c -I  -Werror -g map.cpp 

characterQueue.o: characterQueue.cpp characterQueue.h
	g++ -c -I -Werror -g characterQueue.cpp 

priorityQueue.o: priorityQueue.cpp priorityQueue.h
	g++ -c -I  -Werror -g priorityQueue.cpp 

clean:
	rm -f *.o assignment7 *~
