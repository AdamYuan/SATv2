GCC_FLAGS = -Ofast -funsafe-loop-optimizations -lpthread
#GCC_FLAGS = -g -Og -lpthread
EXECUTABLE_NAME = SATv2

$(EXECUTABLE_NAME): \
	main.cpp parser.cpp data.cpp solver.cpp \
	parser.hpp data.hpp solver.hpp config.hpp 
	g++ *.cpp -o $(EXECUTABLE_NAME) $(GCC_FLAGS)

.PHONY: clean

clean:
	rm $(EXECUTABLE_NAME)
