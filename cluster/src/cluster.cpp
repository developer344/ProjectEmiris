#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fstream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <climits>
#include <string>

#include "clusterUtils.h"
#include "kMeans.h"
#include "mathUtils.h"
#include "lloyd.h"
// #include "hypercubeUtils.h"
// #include "HChashTable.h"
// #include "mathUtils.h"

#define DEF_VECTOR_HASH_TABLES 3
#define DEF_VECTOR_HASH_FUNCTIONS 4
#define DEF_MAX_NUM_M_CUBE 10
#define DEF_NUM_CUBE_DIM 3
#define DEF_PROBES 2

#define EXIT_FAIL_INPUT_ERR 2
#define EXIT_FAIL_CONFIG_ERR 3
#define EXIT_FAIL_OUTPUT_ERR 4
#define EXIT_FAIL_METHOD_ERR 5

#define CLASSIC_METHOD 0
#define LSH_METHOD 1
#define HYPERCUBE_METHOD 2

using namespace std;

int main(int argc, char **argv)
{
    std::vector<std::string> found;
    inputData CLData;
    // Initializing with default values, which may change depending on the config file's content
    CLData.number_of_vector_hash_tables = DEF_VECTOR_HASH_TABLES;
    CLData.number_of_vector_hash_functions = DEF_VECTOR_HASH_FUNCTIONS;
    CLData.max_number_M_hypercube = DEF_MAX_NUM_M_CUBE;
    CLData.number_of_hypercube_dimensions = DEF_NUM_CUBE_DIM;
    CLData.number_of_probes = DEF_PROBES;

    for (int i = 0; i < argc; i++)
    {

        if (std::string(argv[i]) == "-i")
        {
            CLData.inputFileName = std::string(argv[i + 1]);
            std::cout << CLData.inputFileName << std::endl;
            found.push_back("inputFile");
        }
        else if (std::string(argv[i]) == "-c")
        {
            CLData.configFileName = std::string(argv[i + 1]);
            std::cout << CLData.configFileName << std::endl;
            found.push_back("configFile");
        }
        else if (std::string(argv[i]) == "-o")
        {
            CLData.outputFileName = std::string(argv[i + 1]);
            std::cout << CLData.outputFileName << std::endl;
            found.push_back("outputFile");
        }
        else if (std::string(argv[i]) == "-complete")
        {
            CLData.complete = true;
            std::cout << "-complete" << std::endl;
            found.push_back("complete");
        }
        else if (std::string(argv[i]) == "-m")
        {
            CLData.methodName = argv[i + 1];
            std::cout << CLData.methodName << std::endl;
            found.push_back("m");
        }
    }

    found.push_back(" ");

    if (std::find(found.begin(), found.end(), "inputFile") == found.end()) // if not found inputFile
    {
        std::cerr << "Input file name not given! Please try again using -i <input file>" << std::endl;
        return EXIT_FAIL_INPUT_ERR;
    }
    if (std::find(found.begin(), found.end(), "configFile") == found.end()) // if not found inputFile
    {
        std::cerr << "Config file name not given! Please try again using -c <config file>" << std::endl;
        return EXIT_FAIL_CONFIG_ERR;
    }
    if (std::find(found.begin(), found.end(), "outputFile") == found.end()) // if not found inputFile
    {
        std::cerr << "Output file name not given! Please try again using -o <output file>" << std::endl;
        return EXIT_FAIL_OUTPUT_ERR;
    }
    if (std::find(found.begin(), found.end(), "m") == found.end()) // if not found inputFile
    {
        std::cerr << "Method name not given! Please try again using -m <Classic OR LSH OR Hypercube>" << std::endl;
        return EXIT_FAIL_METHOD_ERR;
    }
    else
    {
        if (CLData.methodName == "Classic")
        {
            CLData.method = CLASSIC_METHOD;
        }
        else if (CLData.methodName == "LSH")
        {
            CLData.method = LSH_METHOD;
        }
        else if (CLData.methodName == "Hypercube")
        {
            CLData.method = HYPERCUBE_METHOD;
        }
        else
        {
            std::cerr << "Invalid method name! Please try again using -m <Classic OR LSH OR Hypercube>" << std::endl;
            return EXIT_FAIL_METHOD_ERR;
        }
    }
    found.clear();

    ifstream configFile(CLData.configFileName);
    if (!configFile.is_open())
    {
        cerr << "Could not open the file: '"
             << CLData.configFileName << "'"
             << std::endl;
        return EXIT_FAIL_CONFIG_ERR;
    }
    cout << "Reading config file " << CLData.configFileName << "..." << endl;
    std::vector<std::string> configLines;
    std::string line;

    while (getline(configFile, line))
    {
        // inputLines.push_back(line);
        std::string word = "";
        std::string parameter = "";
        int value = -1;
        for (char x : line)
        {
            if (x == ' ')
            {
                if (parameter.empty())
                {
                    word.pop_back();
                    parameter = word;
                    found.push_back(word);
                }
                else if (value == -1)
                {
                    if (!is_number(word))
                    {
                        cerr << "Parameter [" << parameter << "]: Value '" << word << "' is not an integer" << endl;
                        return EXIT_FAIL_CONFIG_ERR;
                    }

                    value = stoi(word);
                    break;
                }
                word = "";
            }
            else
            {
                word = word + x;
            }
        }

        if (parameter == "number_of_clusters")
        {
            CLData.number_of_clusters = value;
        }
        else if (parameter == "number_of_vector_hash_tables")
        {
            CLData.number_of_vector_hash_tables = value;
        }
        else if (parameter == "number_of_vector_hash_functions")
        {
            CLData.number_of_vector_hash_functions = value;
        }
        else if (parameter == "max_number_M_hypercube")
        {
            CLData.max_number_M_hypercube = value;
        }
        else if (parameter == "number_of_hypercube_dimensions")
        {
            CLData.number_of_hypercube_dimensions = value;
        }
        else if (parameter == "number_of_probes")
        {
            CLData.number_of_probes = value;
        }
    }
    configFile.close();

    found.push_back(" ");

    if (std::find(found.begin(), found.end(), "number_of_clusters") == found.end()) // if not found number_of_clusters
    {
        std::cerr << "Config file must contain parameter [number_of_clusters]."
                  << std::endl
                  << "Please include 'number_of_clusters: <int>' in the config file..."
                  << std::endl;
        return EXIT_FAIL_CONFIG_ERR;
    }

    ifstream inputFile(CLData.inputFileName);
    if (!inputFile.is_open())
    {
        cerr << "Could not open the file: '"
             << CLData.inputFileName << "'"
             << std::endl;
        return EXIT_FAIL_INPUT_ERR;
    }
    cout << "Reading input file " << CLData.inputFileName << "..." << endl;
    std::vector<std::string> inputLines;
    // std::string line;
    while (getline(inputFile, line))
    {
        inputLines.push_back(line);
    }
    inputFile.close();

    std::vector<PointPtr> inputPoints;

    int numOfPoints = inputLines.size();
    std::cout << numOfPoints << std::endl;
    int dimension = 0;

    for (int i = 0; i < numOfPoints; i++)
    {
        // separate std::string by Tabs
        // pick every element from 2nd to std::endl
        // read point coordinates
        Point *currPoint = new Point;
        std::string word = "";
        dimension = 0;
        for (char x : inputLines[i])
        {
            if (x == ' ')
            {
                if (dimension)
                    currPoint->coords.push_back(atof(word.c_str()));
                else
                    currPoint->id = word;
                word = "";

                dimension++;
            }
            else
            {
                word = word + x;
            }
        }

        inputPoints.push_back(currPoint);
    }

    dimension--;
    CLData.dimension = dimension;
    std::cout << "Dimension:" << CLData.dimension << std::endl;

    //Calculate vector of centroid points (1 for each cluster)
    std::vector<PointPtr> centroidPoints = k_means(inputPoints, CLData.number_of_clusters, CLData.dimension);

    std::vector<Cluster> clusters;
    
    clusters.resize(CLData.number_of_clusters);
    for(int i=0;i<CLData.number_of_clusters;i++) {
        clusters[i].centroidPoint = centroidPoints[i];
        clusters[i].size = 0;
    }

    auto cluster_start = std::chrono::high_resolution_clock::now();
    for(int i=0;i<numOfPoints;i++) {
        int index=0;
        if(CLData.method == CLASSIC_METHOD) 
            index = lloyd_method(centroidPoints, inputPoints[i], CLData.dimension);
        else if(CLData.method == LSH_METHOD)
            index = lloyd_method(centroidPoints, inputPoints[i], CLData.dimension);
        else if(CLData.method == HYPERCUBE_METHOD)
            index = lloyd_method(centroidPoints, inputPoints[i], CLData.dimension);
        clusters[index].points.push_back(inputPoints[i]);
        clusters[index].size++;
    }
    auto cluster_end = std::chrono::high_resolution_clock::now();
    int tCluster = std::chrono::duration_cast<std::chrono::milliseconds>(cluster_end - cluster_start).count();

    ofstream outputFile(CLData.outputFileName);
    if (!outputFile.is_open())
    {
        cerr << "Could not open the file: '"
                << CLData.outputFileName << "'"
                << std::endl;
        return EXIT_FAIL_OUTPUT_ERR;
    }

    outputFile << "Algorithm: ";
    if(CLData.method == CLASSIC_METHOD)
        outputFile << "Lloyds";
    else if(CLData.method == LSH_METHOD)
        outputFile << "Range Search LSH";
    else if(CLData.method == HYPERCUBE_METHOD)
        outputFile << "Range Search Hypercube";
    outputFile << endl;

    for (int i = 0; i < CLData.number_of_clusters; i++)
    {
        outputFile << "CLUSTER-"
                    << i+1 << "{size: " << clusters[i].size
                    << ", centroid: ";

        for (int j = 0; j < CLData.dimension; j++)
            outputFile << centroidPoints[i]->coords[j] << " ";
        outputFile << "}" << endl;
    }
    outputFile << "clustering_time: " << (double)(tCluster/1000)
                << "s" << endl;
    outputFile.close();

    return EXIT_SUCCESS;
}