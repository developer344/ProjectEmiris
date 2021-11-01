#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fstream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <climits>

#include "hashTable.h"
#include "mathUtils.h"
#include "lshUtils.h"

#define DEF_K 4
#define DEF_L 5
#define DEF_N 1
#define DEF_R 500

using namespace std;

int main(int argc, char **argv)
{
    std::string option;
    while (!(option == "TERM"))
    {
        std::cout << std::endl
                  << std::endl
                  << "STARTING" << std::endl;
        vector<std::string> found;
        inputData LSHData;
        bool distance_true_visible = false;

        for (int i = 0; i < argc; i++)
        {

            if (std::string(argv[i]) == "-i")
            {
                LSHData.inputFileName = std::string(argv[i + 1]);
                std::cout << LSHData.inputFileName << std::endl;
                found.push_back("inputFile");
            }
            else if (std::string(argv[i]) == "-q")
            {
                LSHData.queryFileName = std::string(argv[i + 1]);
                std::cout << LSHData.queryFileName << std::endl;
                found.push_back("queryFile");
            }
            else if (std::string(argv[i]) == "-o")
            {
                LSHData.outputFileName = std::string(argv[i + 1]);
                std::cout << LSHData.outputFileName << std::endl;
                found.push_back("outputFile");
            }
            else if (std::string(argv[i]) == "-k")
            {
                LSHData.numberOfHyperplanes = atoi(argv[i + 1]);
                std::cout << LSHData.numberOfHyperplanes << std::endl;
                found.push_back("k");
            }
            else if (std::string(argv[i]) == "-L")
            {
                LSHData.intL = atoi(argv[i + 1]);
                std::cout << LSHData.intL << std::endl;
                found.push_back("l");
            }
            else if (std::string(argv[i]) == "-N")
            {
                LSHData.numberOfNearest = atoi(argv[i + 1]);
                std::cout << LSHData.numberOfNearest << std::endl;
                found.push_back("n");
            }
            else if (std::string(argv[i]) == "-R")
            {
                LSHData.radius = atoi(argv[i + 1]);
                std::cout << LSHData.radius << std::endl;
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

        argc = 1;

        found.push_back(" ");
        std::string input = {};

        if (std::find(found.begin(), found.end(), "inputFile") == found.end()) // if not found inputFile
        {
            std::cout << "Please give input file name" << std::endl;

            while ((ch = getchar()) != '\n')
                word += ch;
            LSHData.inputFileName = word;
        }
        if (std::find(found.begin(), found.end(), "queryFile") == found.end()) // if not found queryFile
        {
            std::cout << "Please give query file name" << std::endl;

            while ((ch = getchar()) != '\n')
                word += ch;
            LSHData.queryFileName = word;
        }
        if (std::find(found.begin(), found.end(), "outputFile") == found.end()) // if not found outputFile
        {
            std::cout << "Please give output file name" << std::endl;

            while ((ch = getchar()) != '\n')
                word += ch;
            LSHData.outputFileName = word;
        }
        if (std::find(found.begin(), found.end(), "k") == found.end()) // if not found inputFile
        {
            std::cout << "Please give k: Press [ENTER] for default value or type the desired value." << std::endl;
            LSHData.numberOfHyperplanes = DEF_K;
            word = "";
            while ((ch = getchar()) != '\n')
                word += ch;
            if (word == "")
                std::cout << "Using Default Value of k = " << LSHData.numberOfHyperplanes << endl;
            else
                LSHData.numberOfHyperplanes = stoi(word);
        }
        if (std::find(found.begin(), found.end(), "l") == found.end()) // if not found inputFile
        {
            std::cout << "Please give L: Press [ENTER] for default value or type the desired value." << std::endl;
            LSHData.intL = DEF_L;
            word = "";
            while ((ch = getchar()) != '\n')
                word += ch;
            if (word == "")
                std::cout << "Using Default Value of L = " << LSHData.intL << endl;
            else
                LSHData.intL = stoi(word);
        }
        if (std::find(found.begin(), found.end(), "n") == found.end()) // if not found inputFile
        {
            std::cout << "Please give N: Press [ENTER] for default value or type the desired value." << std::endl;
            LSHData.numberOfNearest = DEF_N;
            word = "";
            while ((ch = getchar()) != '\n')
                word += ch;
            if (word == "")
                std::cout << "Using Default Value of N = " << LSHData.numberOfNearest << endl;
            else
                LSHData.numberOfNearest = stoi(word);
        }
        if (std::find(found.begin(), found.end(), "r") == found.end()) // if not found inputFile
        {
            std::cout << "Please give R: Press [ENTER] for default value or type the desired value." << std::endl;
            LSHData.radius = DEF_R;
            word = "";
            while ((ch = getchar()) != '\n')
                word += ch;
            if (word == "")
                std::cout << "Using Default Value of radius = " << LSHData.radius << endl;
            else
                LSHData.radius = stoi(word);
        }

        if (LSHData.inputFileName.empty() || LSHData.outputFileName.empty() || LSHData.queryFileName.empty())
        {
            cerr << "Arguments must contain all input file, output file and query file. The rest of the arguments are optional"
                 << std::endl;
            return EXIT_FAILURE;
        }
        ifstream inputFile(LSHData.inputFileName);
        if (!inputFile.is_open())
        {
            cerr << "Could not open the file: '"
                 << LSHData.inputFileName << "'"
                 << std::endl;
            return EXIT_FAILURE;
        }
        cout << "Reading input file " << LSHData.inputFileName << "..." << endl;
        vector<std::string> inputLines;
        std::string line;
        while (getline(inputFile, line))
        {
            inputLines.push_back(line);
        }
        inputFile.close();

        vector<PointPtr> inputPoints;

        int numOfPoints = inputLines.size();
        int dimension = 0;

        for (int i = 0; i < numOfPoints; i++)
        {
            // separate std::string by Tabs
            // pick every element from 2nd to std::endl
            // read point coordinates
            PointPtr currPoint = new Point;
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
        LSHData.dimension = dimension;
        std::cout << "dim" << dimension;

        HashTables HashTablesObject(LSHData.intL, LSHData.numberOfHyperplanes, numOfPoints, LSHData.dimension);

        for (int i = 0; i < inputPoints.size(); i++)
            HashTablesObject.HashTables::InsertPoint(inputPoints[i]);

        ifstream queryFile(LSHData.queryFileName);
        if (!queryFile.is_open())
        {
            cerr << "Could not open the file: '"
                 << LSHData.queryFileName << "'"
                 << std::endl;
            return EXIT_FAILURE;
        }

        cout << "Reading query file " << LSHData.queryFileName << "..." << endl;
        vector<std::string> queryLines;
        while (getline(queryFile, line))
        {
            queryLines.push_back(line);
        }
        queryFile.close();

        vector<PointPtr> queryPoints;

        dimension = 0;
        for (int i = 0; i < queryLines.size(); i++)
        {
            // separate std::string by Spaces
            // pick every element from 2nd to std::endl
            // read point coordinates
            PointPtr currPoint = new Point;
            std::string word = "";
            dimension = 0;
            for (char x : queryLines[i])
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

            queryPoints.push_back(currPoint);
        }

        vector<vector<Neighbour> *> k_nearest_neighbours;
        k_nearest_neighbours.resize(queryLines.size());

        vector<kNeighboursPtr> queryOutputData;
        queryOutputData.resize(queryLines.size());

        vector<double> tLSH;
        tLSH.resize(queryLines.size());

        std::cout << "Executing LSH search algorithm..." << std::endl;

        for (int i = 0; i < queryLines.size(); i++)
        {

            auto LSH_start = std::chrono::high_resolution_clock::now();
            queryOutputData[i] = HashTablesObject.HashTables::find_k_nearest_neighbours(queryPoints[i], LSHData.numberOfNearest);
            auto LSH_end = std::chrono::high_resolution_clock::now();
            tLSH[i] = std::chrono::duration_cast<std::chrono::milliseconds>(LSH_end - LSH_start).count();
        }
        vector<kNeighboursPtr> queryTrueNeighbors;
        queryTrueNeighbors.resize(queryLines.size());
        vector<double> tTrue;
        tTrue.resize(queryLines.size());
        std::cout << "Executing brute-force search algorithm..." << std::endl;

        for (int i = 0; i < queryLines.size(); i++)
        {
            auto True_start = std::chrono::high_resolution_clock::now();
            queryTrueNeighbors[i] = find_k_true_neighbours(queryPoints[i], LSHData.numberOfNearest, inputPoints, LSHData.dimension);
            auto True_end = std::chrono::high_resolution_clock::now();
            tTrue[i] = std::chrono::duration_cast<std::chrono::milliseconds>(True_end - True_start).count();
        }
        vector<vector<PointPtr>> queryRangeSearch;
        queryRangeSearch.resize(queryLines.size());

        std::cout << "Executing range search algorithm..." << std::endl;

        for (int i = 0; i < queryLines.size(); i++)
        {
            queryRangeSearch[i] = HashTablesObject.HashTables::range_search(queryPoints[i], LSHData.radius);
        }
        // std::cout << "After kNeighbours" << std::endl;
        ofstream outputFile(LSHData.outputFileName);
        if (!outputFile.is_open())
        {
            cerr << "Could not open the file: '"
                 << LSHData.outputFileName << "'"
                 << std::endl;
            return EXIT_FAILURE;
        }

        for (int i = 0; i < queryLines.size(); i++)
        {
            outputFile << "Query: "
                       << queryPoints[i]->id << std::endl;

            for (int j = 0; j < queryOutputData[i]->size; j++)
            {
                outputFile << "Nearest neighbor-"
                           << j + 1 << ": " << queryOutputData[i]->neighbours[j]->point->id << std::endl
                           << "distanceLSH: " << queryOutputData[i]->neighbours[j]->dist << std::endl;
                if (distance_true_visible)
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

        for (int i = 0; i < inputPoints.size(); i++)
        {
            delete inputPoints[i];
            if (i < queryPoints.size()) // Query points will always be <= input points, so this is safe
            {
                delete queryPoints[i];
                delete k_nearest_neighbours[i];
                for (int j = 0; j < queryOutputData[i]->size; j++)
                {
                    delete queryOutputData[i]->neighbours[j];
                }
                delete queryOutputData[i];

                for (int j = 0; j < queryTrueNeighbors[i]->size; j++)
                {
                    delete queryTrueNeighbors[i]->neighbours[j];
                }
                delete queryTrueNeighbors[i];
            }
        }

        std::cout << "Rerun Program?..." << std::endl
                  << "======Options======" << std::endl
                  << "CONT to rerun" << std::endl
                  << "TERM to terminate" << std::endl
                  << "===================" << std::endl;
        while (true)
        {

            option = "";
            while ((ch = getchar()) != '\n')
                option += ch;
            if (option == "CONT" || option == "TERM")
                break;
        }
    }
    return EXIT_SUCCESS;
}