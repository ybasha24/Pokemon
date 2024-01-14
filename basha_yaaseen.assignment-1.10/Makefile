all: ttc assignment10

ttc: ttc.o
	g++ -o ttc ttc.o 

assignment10: characterQueue.o  priorityQueue.o  map.o db_parse.o  worldGenerator.o 
	g++ -o assignment10 characterQueue.o  priorityQueue.o map.o db_parse.o  worldGenerator.o -lncurses 

ttc.o: ttc.cpp
	g++ -c -I -g ttc.cpp

worldGenerator.o: worldGenerator.cpp db_parse.o worldGenerator.h pokemon.h
	g++ -c -I  -g worldGenerator.cpp

map.o: map.cpp worldGenerator.h
	g++ -c -I  -Werror -g map.cpp 

characterQueue.o: characterQueue.cpp characterQueue.h
	g++ -c -I -Werror -g characterQueue.cpp 

priorityQueue.o: priorityQueue.cpp priorityQueue.h
	g++ -c -I  -Werror -g priorityQueue.cpp 

db_parse.o: db_parse.cpp db_parse.h
	g++ -c -I -Werror -g db_parse.cpp

clean:
	rm -f *.o assignment10 ttc  *~
