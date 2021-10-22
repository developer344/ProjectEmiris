INC=./inc
SRC=./src
CCFLAGS=-lm

TARGETS = lsh.out

lsh.out: $(SRC)/lsh.cpp 
	g++ $(CCFLAGS) -o bin/lsh.out $(SRC)/lsh.cpp $(SRC)/hashTable.cpp $(SRC)/randomGenerators.cpp -I $(INC)

clean:
	rm -r bin/*