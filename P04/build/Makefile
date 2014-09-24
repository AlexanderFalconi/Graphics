all: main.o dllist.o universe.o object.o
	g++ -Wall -std=c++0x main.o dllist.o universe.o object.o -o ../bin/main -lGLEW -lGL -lglut -lGLU

main.o: ../src/main.cpp
	g++ -c ../src/main.cpp

dllist.o: ../src/dllist.cpp
	g++ -c ../src/dllist.cpp

universe.o: ../src/universe.cpp
	g++ -c ../src/universe.cpp

object.o: ../src/object.cpp
	g++ -c ../src/object.cpp

clean:
	rm -rf *o ../bin/main