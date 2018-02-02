GCC_FLAGS = -Ofast

SATv2: \
	main.o parser.o data.o solver.o \
	parser.hpp data.hpp solver.hpp config.hpp
	g++ *.o -o SATv2 $(GCC_FLAGS)

main.o: main.cpp solver.hpp
	g++ -c main.cpp $(GCC_FLAGS) -o main.o

parser.o: parser.cpp parser.hpp config.hpp
	g++ -c parser.cpp $(GCC_FLAGS) -o parser.o

data.o: data.cpp data.hpp config.hpp
	g++ -c data.cpp $(GCC_FLAGS) -o data.o

solver.o: solver.cpp solver.hpp config.hpp
	g++ -c solver.cpp $(GCC_FLAGS) -o solver.o

.PHONY: clean

clean:
	rm *.o SATv2
