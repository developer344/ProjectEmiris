INC=./inc
SRC=./src
CCFLAGS=-lm -g

TARGETS = cluster.out

cluster.out: $(SRC)/cluster.cpp 
	g++ $(CCFLAGS) -o bin/cluster.out $(SRC)/cluster.cpp $(SRC)/clusterUtils.cpp $(SRC)/kMeans.cpp $(SRC)/mathUtils.cpp $(SRC)/lloyd.cpp -I $(INC)

clean:
	rm -r bin/*