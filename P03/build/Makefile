CC=g++
CFLAGS=-Wall -std=c++0x
LDFLAGS=
SOURCES=../src/main.cpp
OBJECTS=$(SOURCES: .cpp=.o)
EXECUTABLE=../bin/main

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) -o $@ -lGLEW -lGL -lglut -lGLU

clean:
	rm Makefile~
