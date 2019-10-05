
CC = g++
SOURCES = lingeolib.cpp
MAIN_EXEC = tri_square_main.cpp
MAIN_EXECO = $(MAIN_EXEC: .cpp=.o)
MAIN_TESTER = tester.cpp
MAIN_TESTERO = $(MAIN_TESTER: .cpp=.o)
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = sq_inter
TESTER = tester

all: $(SOURCES) $(MAIN_EXEC) $(EXECUTABLE) $(TESTER)



$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(MAIN_EXECO) -o $@

.cpp.o:
	$(CC) $(DEBUG) $(FLAGS) -c -o $@ $<


clean:
	rm -rf *.o $(EXECUTABLE) $(TESTER)

$(TESTER): $(MAIN_TESTER)
	$(CC) $(DEBUG) $(OBJECTS) $(MAIN_TESTERO) -o $@