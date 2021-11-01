#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fstream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <climits>

#include "hypercubeUtils.h"
#include "HChashTable.h"
#include "mathUtils.h"

#define DEF_K 14
#define DEF_M 10
#define DEF_PROBES 2
#define DEF_N 1
#define DEF_R 500

using namespace std;

int main(int argc, char **argv)
{
    std::string option;
    while (!(option == "TERM" || option == "term"))
    {
        std::cout << std::endl
                  << std::endl
                  << "STARTING" << std::endl;
        vector<std::string> found;
        inputData HCData;
        bool distance_true_visible = false;

        for (int i = 0; i < argc; i++)
        {

            if (std::string(argv[i]) == "-i")
            {
                HCData.inputFileName = std::string(argv[i + 1]);
                std::cout << HCData.inputFileName << std::endl;
                found.push_back("inputFile");
            }
            else if (std::string(argv[i]) == "-q")
            {
                HCData.queryFileName = std::string(argv[i + 1]);
                std::cout << HCData.queryFileName << std::endl;
                found.push_back("queryFile");
            }
            else if (std::string(argv[i]) == "-o")
            {
                HCData.outputFileName = std::string(argv[i + 1]);
                std::cout << HCData.outputFileName << std::endl;
                found.push_back("outputFile");
            }
            else if (std::string(argv[i]) == "-k")
            {
                HCData.projectionDimension = atoi(argv[i + 1]);
                std::cout << HCData.projectionDimension << std::endl;
                found.push_back("k");
            }
            else if (std::string(argv[i]) == "-M")
            {
                HCData.maxCandidatePoints = atoi(argv[i + 1]);
                std::cout << HCData.maxCandidatePoints << std::endl;
                found.push_back("m");
            }
            else if (std::string(argv[i]) == "-N")
            {
                HCData.numberOfNearest = atoi(argv[i + 1]);
                std::cout << HCData.numberOfNearest << std::endl;
                found.push_back("n");
            }
            else if (std::string(argv[i]) == "-probes")
            {
                HCData.probes = atoi(argv[i + 1]);
                std::cout << HCData.probes << std::endl;
                found.push_back("probes");
            }
            else if (std::string(argv[i]) == "-R")
            {
                HCData.radius = atoi(argv[i + 1]);
                std::cout << HCData.radius << std::endl;
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
            HCData.inputFileName = word;
        }
        if (std::find(found.begin(), found.end(), "queryFile") == found.end()) // if not found queryFile
        {
            std::cout << "Please give query file name" << std::endl;

            while ((ch = getchar()) != '\n')
                word += ch;
            HCData.queryFileName = word;
        }
        if (std::find(found.begin(), found.end(), "outputFile") == found.end()) // if not found outputFile
        {
            std::cout << "Please give output file name" << std::endl;

            while ((ch = getchar()) != '\n')
                word += ch;
            HCData.outputFileName = word;
        }
        if (std::find(found.begin(), found.end(), "k") == found.end()) // if not found inputFile
        {
            std::cout << "Please give k: Press [ENTER] for default value or type the desired value." << std::endl;
            HCData.projectionDimension = DEF_K;
            word = "";
            while ((ch = getchar()) != '\n')
                word += ch;
            if (word == "")
                std::cout << "Using Default Value of k = " << HCData.projectionDimension << endl;
            else
                HCData.projectionDimension = stoi(word);
        }
        if (std::find(found.begin(), found.end(), "m") == found.end()) // if not found inputFile
        {
            std::cout << "Please give M: Press [ENTER] for default value or type the desired value." << std::endl;
            HCData.maxCandidatePoints = DEF_M;
            word = "";
            while ((ch = getchar()) != '\n')
                word += ch;
            if (word == "")
                std::cout << "Using Default Value of M = " << HCData.maxCandidatePoints << endl;
            else
                HCData.maxCandidatePoints = stoi(word);
        }
        if (std::find(found.begin(), found.end(), "probes") == found.end()) // if not found inputFile
        {
            std::cout << "Please give number of probes: Press [ENTER] for default value or type the desired value." << std::endl;
            HCData.probes = DEF_PROBES;
            word = "";
            while ((ch = getchar()) != '\n')
                word += ch;
            if (word == "")
                std::cout << "Using Default Value of probes = " << HCData.probes << endl;
            else
                HCData.probes = stoi(word);
        }
        if (std::find(found.begin(), found.end(), "n") == found.end()) // if not found inputFile
        {
            std::cout << "Please give N: Press [ENTER] for default value or type the desired value." << std::endl;
            HCData.numberOfNearest = DEF_N;
            word = "";
            while ((ch = getchar()) != '\n')
                word += ch;
            if (word == "")
                std::cout << "Using Default Value of N = " << HCData.numberOfNearest << endl;
            else
                HCData.numberOfNearest = stoi(word);
        }
        if (std::find(found.begin(), found.end(), "r") == found.end()) // if not found inputFile
        {
            std::cout << "Please give R: Press [ENTER] for default value or type the desired value." << std::endl;
            HCData.radius = DEF_R;
            word = "";
            while ((ch = getchar()) != '\n')
                word += ch;
            if (word == "")
                std::cout << "Using Default Value of radius = " << HCData.radius << endl;
            else
                HCData.radius = stoi(word);
        }
        ///////////////////////////////////////////////////////////////////////////////////////////////////////
        if (HCData.inputFileName.empty() || HCData.outputFileName.empty() || HCData.queryFileName.empty())
        {
            cerr << "Arguments must contain all input file, output file and query file. The rest of the arguments are optional"
                 << std::endl;
            return EXIT_FAILURE;
        }
        ifstream inputFile(HCData.inputFileName);
        if (!inputFile.is_open())
        {
            cerr << "Could not open the file: '"
                 << HCData.inputFileName << "'"
                 << std::endl;
            return EXIT_FAILURE;
        }
        cout << "Reading input file " << HCData.inputFileName << "..." << endl;
        std::vector<std::string> inputLines;
        std::string line;
        while (getline(inputFile, line))
        {
            inputLines.push_back(line);
        }

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
        inputFile.close();
        dimension--;
        HCData.dimension = dimension;
        std::cout << "Dimension:" << HCData.dimension << std::endl;
        // Here insert points to Hypercube data structure when u figure out how

        HChashTable HypercubeObject(HCData.dimension, HCData.projectionDimension, HCData.probes, HCData.maxCandidatePoints);
        std::cout << "Inserting items to hash table..." << std::endl;
        for (int i = 0; i < inputPoints.size(); i++)
            HypercubeObject.HChashTable::InsertPoint(inputPoints[i]);

        ifstream queryFile;
        queryFile.open(HCData.queryFileName, ios::in);
        if (!queryFile.is_open())
        {
            cerr << "Could not open the file: '"
                 << HCData.queryFileName << "'"
                 << std::endl;
            return EXIT_FAILURE;
        }

        cout << "Reading query file " << HCData.queryFileName << "..." << endl;
        vector<std::string> queryLines;
        while (getline(queryFile, line))
        {
            queryLines.push_back(line);
        }

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
        queryFile.close();

        vector<vector<Neighbour> *> k_nearest_neighbours;
        k_nearest_neighbours.resize(queryLines.size());

        vector<kNeighboursPtr> queryOutputData;
        queryOutputData.resize(queryLines.size());

        vector<double> tCube;
        tCube.resize(queryLines.size());

        std::cout << "Executing Hypercube search algorithm..." << std::endl;

        for (int i = 0; i < queryLines.size(); i++)
        {
            auto Cube_start = std::chrono::high_resolution_clock::now();
            queryOutputData[i] = HypercubeObject.HChashTable::find_k_nearest_neighbours(queryPoints[i], HCData.numberOfNearest);
            auto Cube_end = std::chrono::high_resolution_clock::now();
            tCube[i] = std::chrono::duration_cast<std::chrono::milliseconds>(Cube_end - Cube_start).count();
        }

        vector<kNeighboursPtr> queryTrueNeighbors;
        queryTrueNeighbors.resize(queryLines.size());
        vector<double> tTrue;
        tTrue.resize(queryLines.size());
        std::cout << "Executing brute-force search algorithm..." << std::endl;

        for (int i = 0; i < queryLines.size(); i++)
        {
            auto True_start = std::chrono::high_resolution_clock::now();
            queryTrueNeighbors[i] = find_k_true_neighbours(queryPoints[i], HCData.numberOfNearest, inputPoints, HCData.dimension);
            auto True_end = std::chrono::high_resolution_clock::now();
            tTrue[i] = std::chrono::duration_cast<std::chrono::milliseconds>(True_end - True_start).count();
        }

        vector<vector<PointPtr>> queryRangeSearch;
        queryRangeSearch.resize(queryLines.size());

        std::cout << "Executing range search algorithm..." << std::endl;

        for (int i = 0; i < queryLines.size(); i++)
        {
            queryRangeSearch[i] = HypercubeObject.HChashTable::range_search(queryPoints[i], HCData.radius);
        }

        ofstream outputFile(HCData.outputFileName);
        if (!outputFile.is_open())
        {
            cerr << "Could not open the file: '"
                 << HCData.outputFileName << "'"
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
                           << "distanceHypercube: " << queryOutputData[i]->neighbours[j]->dist << std::endl;
                if (distance_true_visible)
                {
                    outputFile << "True Nearest neighbor-"
                               << j + 1 << ": " << queryTrueNeighbors[i]->neighbours[j]->point->id << std::endl;
                }
                outputFile << "distanceTrue: " << queryTrueNeighbors[i]->neighbours[j]->dist << std::endl;
            }

            outputFile << "tCube: " << (double)(tCube[i] / 1000) << 's' << std::endl
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
                for (int j = 0; j < queryOutputData[i]->neighbours.size(); j++)
                {
                    delete queryOutputData[i]->neighbours[j];
                }
                delete queryOutputData[i];

                for (int j = 0; j < queryTrueNeighbors[i]->neighbours.size(); j++)
                {
                    delete queryTrueNeighbors[i]->neighbours[j];
                }
                delete queryTrueNeighbors[i];
            }
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////
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
            if (option == "CONT" || option == "TERM" || option == "term")
                break;
        }
    }
    return EXIT_SUCCESS;
}
