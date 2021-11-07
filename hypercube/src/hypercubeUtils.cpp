#include "hypercubeUtils.h"
#include <time.h>
#include <algorithm>
#include <stdlib.h>
#include <fstream>
#include <random>

using namespace std;

std::vector<std::string> get_lines(std::string fileName)
{
    ifstream file(fileName);
    if (!file.is_open())
    {
        std::cerr << "Could not open the file: '"
                  << fileName << "'"
                  << std::endl;
        std::exit(EXIT_FAILURE);
    }
    std::cout << "Reading input file " << fileName << "..." << std::endl;
    std::vector<std::string> inputLines;
    std::string line;
    while (getline(file, line))
    {
        inputLines.push_back(line);
    }
    file.close();
    return inputLines;
}

int get_points(vector<std::string> linesVector, vector<PointPtr> *pointsVector)
{
    int dimension;
    for (int i = 0; i < linesVector.size(); i++)
    {
        // separate std::string by Spaces
        // pick every element from 2nd to std::endl
        // read point coordinates
        PointPtr currPoint = new Point;
        std::string word = "";
        dimension = 0;
        for (char x : linesVector[i])
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

        pointsVector->push_back(currPoint);
    }
    dimension--;
    return dimension;
}

void sort_neighbours(kNeighboursPtr k_nearest_neighbours, int k_neighbours) // sort distance in a vector of k distances
{
    int k = k_neighbours; // number of neighbours
    NeighbourPtr tempNeighbour;

    for (int i = k - 1; i > 0; i--)
    {
        if (k_nearest_neighbours->neighbours[i]->dist < k_nearest_neighbours->neighbours[i - 1]->dist)
        {
            tempNeighbour = k_nearest_neighbours->neighbours[i - 1];
            k_nearest_neighbours->neighbours[i - 1] = k_nearest_neighbours->neighbours[i];
            k_nearest_neighbours->neighbours[i] = tempNeighbour;
        }
    }
}

void sort_points(std::vector<PointPtr> *Data) // sort distance in a vector of k distances
{
    int k = Data->size();
    PointPtr tempPoint;
    for (int i = k - 1; i > 0; i--)
    {
        if ((*Data)[i]->id < (*Data)[i - 1]->id)
        {
            tempPoint = (*Data)[i];
            (*Data)[i] = (*Data)[i - 1];
            (*Data)[i - 1] = tempPoint;
        }
    }
}

int notAlreadyExists(kNeighboursPtr k_nearest_neighbours, std::string pointID)
{

    for (int i = 0; i < k_nearest_neighbours->size; i++)
        if (k_nearest_neighbours->neighbours[i]->point->id == pointID)
            return 0;
    return 1;
}

kNeighboursPtr find_k_true_neighbours(PointPtr queryPoint, int k_neighbours, std::vector<PointPtr> inputPoints, int dim)
{
    // PointPtr curPoint;
    // int curDist;

    NeighbourPtr currNeighbour = new Neighbour;

    kNeighboursPtr returnData = new kNeighbours;
    returnData->neighbours.resize(k_neighbours);
    // returnData->size = 0;
    returnData->size = k_neighbours;

    for (int i = 0; i < k_neighbours; i++)
    {
        returnData->neighbours[i] = new Neighbour;
        returnData->neighbours[i]->point = NULL;
        returnData->neighbours[i]->dist = INT32_MAX; // initialize distance with a very big value
    }

    for (int i = 0; i < inputPoints.size(); i++)
    {
        currNeighbour->point = inputPoints[i];
        currNeighbour->dist = euclideanDistance(queryPoint, currNeighbour->point, dim);

        if (currNeighbour->dist < returnData->neighbours[k_neighbours - 1]->dist && currNeighbour->dist > 0)
        {
            // if (returnData->size < k_neighbours)
            //     returnData->size++;
            returnData->neighbours[k_neighbours - 1]->point = currNeighbour->point;
            returnData->neighbours[k_neighbours - 1]->dist = currNeighbour->dist;

            if (k_neighbours > 1)
                sort_neighbours(returnData, k_neighbours);
        }
    }

    delete currNeighbour;
    return returnData;
}

// maps an integer value (h) to 0 or 1
bool mapFunction(int h, int i)
{
    // srand(time(NULL));
    // int ret = euclideanModulo(h, rand() % 100);
    // if (i % 2 == 0)
    // {
    //     return ret > rand() % 50 ? true : false;
    // }
    // else
    //     return ret <= rand() % 50 ? true : false;

    int ret = euclideanModulo(h, i * 10);

    return ret > i * 5 ? true : false;
}

inputData *getInputData(int *argc, char **argv)
{

    inputData *HCData = new inputData;
    vector<std::string> found;
        bool distance_true_visible = false;

        for (int i = 0; i < *argc; i++)
        {

            if (std::string(argv[i]) == "-i")
            {
                HCData->inputFileName = std::string(argv[i + 1]);
                std::cout << HCData->inputFileName << std::endl;
                found.push_back("inputFile");
            }
            else if (std::string(argv[i]) == "-q")
            {
                HCData->queryFileName = std::string(argv[i + 1]);
                std::cout << HCData->queryFileName << std::endl;
                found.push_back("queryFile");
            }
            else if (std::string(argv[i]) == "-o")
            {
                HCData->outputFileName = std::string(argv[i + 1]);
                std::cout << HCData->outputFileName << std::endl;
                found.push_back("outputFile");
            }
            else if (std::string(argv[i]) == "-k")
            {
                HCData->projectionDimension = atoi(argv[i + 1]);
                std::cout << HCData->projectionDimension << std::endl;
                found.push_back("k");
            }
            else if (std::string(argv[i]) == "-M")
            {
                HCData->maxCandidatePoints = atoi(argv[i + 1]);
                std::cout << HCData->maxCandidatePoints << std::endl;
                found.push_back("m");
            }
            else if (std::string(argv[i]) == "-N")
            {
                HCData->numberOfNearest = atoi(argv[i + 1]);
                std::cout << HCData->numberOfNearest << std::endl;
                found.push_back("n");
            }
            else if (std::string(argv[i]) == "-probes")
            {
                HCData->probes = atoi(argv[i + 1]);
                std::cout << HCData->probes << std::endl;
                found.push_back("probes");
            }
            else if (std::string(argv[i]) == "-R")
            {
                HCData->radius = atoi(argv[i + 1]);
                std::cout << HCData->radius << std::endl;
                found.push_back("r");
            }
            else if (std::string(argv[i]) == "--dist-true=visible")
            {
                distance_true_visible = true;
            }
        }

        //-----------------------------------------------------------------------------------------//

        char ch;
        string word = "";

        *argc = 1;

        found.push_back(" ");
        std::string input = {};

        if (std::find(found.begin(), found.end(), "inputFile") == found.end()) // if not found inputFile
        {
            std::cout << "Please give input file name" << std::endl;

            while ((ch = getchar()) != '\n')
                word += ch;
            HCData->inputFileName = word;
        }
        if (std::find(found.begin(), found.end(), "queryFile") == found.end()) // if not found queryFile
        {
            std::cout << "Please give query file name" << std::endl;

            while ((ch = getchar()) != '\n')
                word += ch;
            HCData->queryFileName = word;
        }
        if (std::find(found.begin(), found.end(), "outputFile") == found.end()) // if not found outputFile
        {
            std::cout << "Please give output file name" << std::endl;

            while ((ch = getchar()) != '\n')
                word += ch;
            HCData->outputFileName = word;
        }
        if (std::find(found.begin(), found.end(), "k") == found.end()) // if not found inputFile
        {
            std::cout << "Please give k: Press [ENTER] for default value or type the desired value." << std::endl;
            HCData->projectionDimension = DEF_K;
            word = "";
            while ((ch = getchar()) != '\n')
                word += ch;
            if (word == "")
                std::cout << "Using Default Value of k = " << HCData->projectionDimension << endl;
            else
                HCData->projectionDimension = stoi(word);
        }
        if (std::find(found.begin(), found.end(), "m") == found.end()) // if not found inputFile
        {
            std::cout << "Please give M: Press [ENTER] for default value or type the desired value." << std::endl;
            HCData->maxCandidatePoints = DEF_M;
            word = "";
            while ((ch = getchar()) != '\n')
                word += ch;
            if (word == "")
                std::cout << "Using Default Value of M = " << HCData->maxCandidatePoints << endl;
            else
                HCData->maxCandidatePoints = stoi(word);
        }
        if (std::find(found.begin(), found.end(), "probes") == found.end()) // if not found inputFile
        {
            std::cout << "Please give number of probes: Press [ENTER] for default value or type the desired value." << std::endl;
            HCData->probes = DEF_PROBES;
            word = "";
            while ((ch = getchar()) != '\n')
                word += ch;
            if (word == "")
                std::cout << "Using Default Value of probes = " << HCData->probes << endl;
            else
                HCData->probes = stoi(word);
        }
        if (std::find(found.begin(), found.end(), "n") == found.end()) // if not found inputFile
        {
            std::cout << "Please give N: Press [ENTER] for default value or type the desired value." << std::endl;
            HCData->numberOfNearest = DEF_N;
            word = "";
            while ((ch = getchar()) != '\n')
                word += ch;
            if (word == "")
                std::cout << "Using Default Value of N = " << HCData->numberOfNearest << endl;
            else
                HCData->numberOfNearest = stoi(word);
        }
        if (std::find(found.begin(), found.end(), "r") == found.end()) // if not found inputFile
        {
            std::cout << "Please give R: Press [ENTER] for default value or type the desired value." << std::endl;
            HCData->radius = DEF_R;
            word = "";
            while ((ch = getchar()) != '\n')
                word += ch;
            if (word == "")
                std::cout << "Using Default Value of radius = " << HCData->radius << endl;
            else
                HCData->radius = stoi(word);
        }
        ///////////////////////////////////////////////////////////////////////////////////////////////////////
        if (HCData->inputFileName.empty() || HCData->outputFileName.empty() || HCData->queryFileName.empty())
        {
            cerr << "Arguments must contain all input file, output file and query file. The rest of the arguments are optional"
                 << std::endl;
            return NULL;
        }
    return HCData;
}

int writeToOutput(inputData *HCData,
                  std::vector<PointPtr> queryPoints,
                  vector<kNeighboursPtr> queryOutputData,
                  vector<kNeighboursPtr> queryTrueNeighbors,
                  vector<vector<PointPtr>> queryRangeSearch,
                  vector<double> tLSH,
                  vector<double> tTrue)
{
    ofstream outputFile(HCData->outputFileName);
    if (!outputFile.is_open())
    {
        cerr << "Could not open the file: '"
             << HCData->outputFileName << "'"
             << std::endl;
        return EXIT_FAILURE;
    }

    for (int i = 0; i < queryPoints.size(); i++)
    {
        outputFile << "Query: "
                   << queryPoints[i]->id << std::endl;

        for (int j = 0; j < queryOutputData[i]->size; j++)
        {
            outputFile << "Nearest neighbor-"
                       << j + 1 << ": " << queryOutputData[i]->neighbours[j]->point->id << std::endl
                       << "distanceLSH: " << queryOutputData[i]->neighbours[j]->dist << std::endl;
            if (HCData->distance_true_visible)
            {
                outputFile << "True Nearest neighbor-"
                           << j + 1 << ": " << queryTrueNeighbors[i]->neighbours[j]->point->id << std::endl;
            }
            outputFile << "distanceTrue: " << queryTrueNeighbors[i]->neighbours[j]->dist << std::endl;
        }

        outputFile << "tLSH: " << (double)(tLSH[i] / 1000) << 's' << std::endl
                   << "tTrue: " << (double)(tTrue[i] / 1000) << 's' << std::endl
                   << "R-near neighbors:" << std::endl;
        for (int j = 0; j < queryRangeSearch[i].size(); j++)
            outputFile << queryRangeSearch[i][j]->id << std::endl;
        outputFile << std::endl
                   << std::endl;
    }
    outputFile.close();
    return EXIT_SUCCESS;
}

void deleteData(std::vector<PointPtr> *inputPoints,
                std::vector<PointPtr> *queryPoints,
                vector<vector<Neighbour> *> *k_nearest_neighbours,
                vector<kNeighboursPtr> *queryOutputData,
                vector<kNeighboursPtr> *queryTrueNeighbors)
{
    for (int i = 0; i < inputPoints->size(); i++)
    {
        delete (*inputPoints)[i];
        if (i < queryPoints->size()) // Query points will always be <= input points, so this is safe
        {
            delete (*queryPoints)[i];
            delete (*k_nearest_neighbours)[i];
            for (int j = 0; j < (*queryOutputData)[i]->size; j++)
            {
                delete (*queryOutputData)[i]->neighbours[j];
            }
            delete (*queryOutputData)[i];

            for (int j = 0; j < (*queryTrueNeighbors)[i]->size; j++)
            {
                delete (*queryTrueNeighbors)[i]->neighbours[j];
            }
            delete (*queryTrueNeighbors)[i];
        }
    }
}