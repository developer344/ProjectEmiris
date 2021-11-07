INC=./inc # obsolete
SRC=./src
LIB=../lib
CCFLAGS=-lm -g
INCLUDES= -I ./inc -I ../lib

TARGETS = cluster.out

cluster.out: $(SRC)/cluster.cpp 
	g++ $(CCFLAGS) -o bin/cluster.out $(SRC)/cluster.cpp $(SRC)/clusterUtils.cpp $(SRC)/hashTable.cpp $(SRC)/HChashTable.cpp $(SRC)/kMeans.cpp $(SRC)/methods.cpp $(LIB)/mathUtils.cpp $(LIB)/projectUtils.cpp $(INCLUDES)

clean:
	rm -r bin/*