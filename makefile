
CC = g++
SOURCES = lingeolib.cpp tri_square_main.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = sq_inter

all: $(SOURCES) $(EXECUTABLE)


$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(DEBUG) $(FLAGS) -c -o $@ $<


clean:
	rm -rf *.o $(EXECUTABLE)