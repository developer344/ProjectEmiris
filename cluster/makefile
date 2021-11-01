INC=./inc
SRC=./src
CCFLAGS=-lm -g

TARGETS = cluster.out

cluster.out: $(SRC)/cluster.cpp 
	g++ $(CCFLAGS) -o bin/cluster.out $(SRC)/cluster.cpp $(SRC)/clusterUtils.cpp -I $(INC)

clean:
	rm -r bin/*