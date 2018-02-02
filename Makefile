GCC_FLAGS = -Ofast
EXECUTABLE_NAME = SATv2

$(EXECUTABLE_NAME): \
	main.cpp parser.cpp data.cpp solver.cpp \
	parser.hpp data.hpp solver.hpp config.hpp 
	g++ *.cpp -o $(EXECUTABLE_NAME) $(GCC_FLAGS)

.PHONY: clean

clean:
	rm $(EXECUTABLE_NAME)
