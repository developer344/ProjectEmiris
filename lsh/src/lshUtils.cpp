#include <iostream>
#include <algorithm>
#include <fstream>

#include "lshUtils.h"

inputData *getInputData(int *argc, char **argv)
{

    inputData *LSHData = new inputData;
    std::vector<std::string> found;
    LSHData->distance_true_visible = false;

    for (int i = 0; i < *argc; i++)
    {

        if (std::string(argv[i]) == "-i")
        {
            LSHData->inputFileName = std::string(argv[i + 1]);
            std::cout << LSHData->inputFileName << std::endl;
            found.push_back("inputFile");
        }
        else if (std::string(argv[i]) == "-q")
        {
            LSHData->queryFileName = std::string(argv[i + 1]);
            std::cout << LSHData->queryFileName << std::endl;
            found.push_back("queryFile");
        }
        else if (std::string(argv[i]) == "-o")
        {
            LSHData->outputFileName = std::string(argv[i + 1]);
            std::cout << LSHData->outputFileName << std::endl;
            found.push_back("outputFile");
        }
        else if (std::string(argv[i]) == "-k")
        {
            LSHData->numberOfHyperplanes = atoi(argv[i + 1]);
            std::cout << LSHData->numberOfHyperplanes << std::endl;
            found.push_back("k");
        }
        else if (std::string(argv[i]) == "-L")
        {
            LSHData->intL = atoi(argv[i + 1]);
            std::cout << LSHData->intL << std::endl;
            found.push_back("l");
        }
        else if (std::string(argv[i]) == "-N")
        {
            LSHData->numberOfNearest = atoi(argv[i + 1]);
            std::cout << LSHData->numberOfNearest << std::endl;
            found.push_back("n");
        }
        else if (std::string(argv[i]) == "-R")
        {
            LSHData->radius = atoi(argv[i + 1]);
            std::cout << LSHData->radius << std::endl;
            found.push_back("r");
        }
        else if (std::string(argv[i]) == "--dist-true=visible")
        {
            LSHData->distance_true_visible = true;
        }
    }

    //-----------------------------------------------------------------------------------------//

    char ch;
    std::string word = "";

    *argc = 1;

    found.push_back(" ");
    std::string input = {};

    if (std::find(found.begin(), found.end(), "inputFile") == found.end()) // if not found inputFile
    {
        std::cout << "Please give input file name" << std::endl;

        while ((ch = getchar()) != '\n')
            word += ch;
        LSHData->inputFileName = word;
    }
    if (std::find(found.begin(), found.end(), "queryFile") == found.end()) // if not found queryFile
    {
        std::cout << "Please give query file name" << std::endl;

        while ((ch = getchar()) != '\n')
            word += ch;
        LSHData->queryFileName = word;
    }
    if (std::find(found.begin(), found.end(), "outputFile") == found.end()) // if not found outputFile
    {
        std::cout << "Please give output file name" << std::endl;

        while ((ch = getchar()) != '\n')
            word += ch;
        LSHData->outputFileName = word;
    }
    if (std::find(found.begin(), found.end(), "k") == found.end()) // if not found inputFile
    {
        std::cout << "Please give k: Press [ENTER] for default value or type the desired value." << std::endl;
        LSHData->numberOfHyperplanes = DEF_K;
        word = "";
        while ((ch = getchar()) != '\n')
            word += ch;
        if (word == "")
            std::cout << "Using Default Value of k = " << LSHData->numberOfHyperplanes << std::endl;
        else
            LSHData->numberOfHyperplanes = stoi(word);
    }
    if (std::find(found.begin(), found.end(), "l") == found.end()) // if not found inputFile
    {
        std::cout << "Please give L: Press [ENTER] for default value or type the desired value." << std::endl;
        LSHData->intL = DEF_L;
        word = "";
        while ((ch = getchar()) != '\n')
            word += ch;
        if (word == "")
            std::cout << "Using Default Value of L = " << LSHData->intL << std::endl;
        else
            LSHData->intL = stoi(word);
    }
    if (std::find(found.begin(), found.end(), "n") == found.end()) // if not found inputFile
    {
        std::cout << "Please give N: Press [ENTER] for default value or type the desired value." << std::endl;
        LSHData->numberOfNearest = DEF_N;
        word = "";
        while ((ch = getchar()) != '\n')
            word += ch;
        if (word == "")
            std::cout << "Using Default Value of N = " << LSHData->numberOfNearest << std::endl;
        else
            LSHData->numberOfNearest = stoi(word);
    }
    if (std::find(found.begin(), found.end(), "r") == found.end()) // if not found inputFile
    {
        std::cout << "Please give R: Press [ENTER] for default value or type the desired value." << std::endl;
        LSHData->radius = DEF_R;
        word = "";
        while ((ch = getchar()) != '\n')
            word += ch;
        if (word == "")
            std::cout << "Using Default Value of radius = " << LSHData->radius << std::endl;
        else
            LSHData->radius = stoi(word);
    }

    if (LSHData->inputFileName.empty() || LSHData->outputFileName.empty() || LSHData->queryFileName.empty())
    {
        std::cerr << "Arguments must contain all input file, output file and query file. The rest of the arguments are optional"
                  << std::endl;
        delete LSHData;
        return NULL;
    }
    return LSHData;
}

int writeToOutput(inputData *LSHData,
                  std::vector<PointPtr> queryPoints,
                  std::vector<kNeighboursPtr> queryOutputData,
                  std::vector<kNeighboursPtr> queryTrueNeighbors,
                  std::vector<std::vector<PointPtr>> queryRangeSearch,
                  std::vector<double> tLSH,
                  std::vector<double> tTrue)
{
    std::ofstream outputFile(LSHData->outputFileName);
    if (!outputFile.is_open())
    {
        std::cerr << "Could not open the file: '"
                  << LSHData->outputFileName << "'"
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
            if (LSHData->distance_true_visible)
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
                std::vector<std::vector<Neighbour> *> *k_nearest_neighbours,
                std::vector<kNeighboursPtr> *queryOutputData,
                std::vector<kNeighboursPtr> *queryTrueNeighbors,
                inputData *LSHData)
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
    delete LSHData;
}