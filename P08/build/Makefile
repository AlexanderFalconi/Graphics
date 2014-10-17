CC=g++
CFLAGS=-Wall -std=c++0x
CXXFLAGS=
LDFLAGS=`Magick++-config --cppflags --cxxflags --ldflags --libs`
SOURCES= ../src/shader.cpp ../src/texture.cpp ../src/mesh.cpp ../src/universe.cpp ../src/object.cpp ../src/main.cpp
OBJECTS=$(SOURCES: .cpp=.o)
EXECUTABLE=../bin/main

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ -lGLEW -lGL -lglut -lGLU -lassimp $(LDFLAGS)

clean:
	rm ../bin/main