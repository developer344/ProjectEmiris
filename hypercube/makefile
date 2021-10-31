INC=./inc
SRC=./src
CCFLAGS=-lm -g

TARGETS = cube.out

cube.out: $(SRC)/cube.cpp 
	g++ $(CCFLAGS) -o bin/cube.out $(SRC)/cube.cpp $(SRC)/HChashTable.cpp $(SRC)/mathUtils.cpp $(SRC)/hypercubeUtils.cpp -I $(INC)

clean:
	rm -r bin/*