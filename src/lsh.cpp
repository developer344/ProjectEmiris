#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fstream>
#include <vector>
#include <chrono>

#include "hashTable.h"
#include "mathUtils.h"
#include "lshUtils.h"

using namespace std;

int main(int argc, char **argv)
{
    //./lsh–i<inputfile> –q<queryfile> –k<int> -L<int> -ο<outputfile> -Ν<numberofnearest> -R<radius>
    // if (!(argc % 2) || argc > 15 || argc < 7)
    // {
    //     cerr << "Incorrect format of arguments"
    //          << endl;
    //     return EXIT_FAILURE;
    // }

    inputData LSHData;
    LSHData.inputFileName = "";
    LSHData.queryFileName = "";
    LSHData.outputFileName = "";
    LSHData.numberOfHyperplanes = 4;
    LSHData.intL = 5;
    LSHData.numberOfNearest = 1;
    LSHData.radius = 500;
    LSHData.dimension = 0;
    int distance_true_visible = 0;

    for (int i = 0; i < argc; i++)
        if (!strcmp(argv[i], "-i"))
        {
            LSHData.inputFileName = string(argv[i + 1]);
            cout << LSHData.inputFileName << endl;
        }
        else if (!strcmp(argv[i], "-q"))
        {
            LSHData.queryFileName = string(argv[i + 1]);
            cout << LSHData.queryFileName << endl;
        }
        else if (!strcmp(argv[i], "-o"))
        {
            LSHData.outputFileName = string(argv[i + 1]);
            cout << LSHData.outputFileName << endl;
        }
        else if (!strcmp(argv[i], "-k"))
        {
            LSHData.numberOfHyperplanes = atoi(argv[i + 1]);
            cout << LSHData.numberOfHyperplanes << endl;
        }
        else if (!strcmp(argv[i], "-L"))
        {
            LSHData.intL = atoi(argv[i + 1]);
            cout << LSHData.intL << endl;
        }
        else if (!strcmp(argv[i], "-N"))
        {
            LSHData.numberOfNearest = atoi(argv[i + 1]);
            cout << LSHData.numberOfNearest << endl;
        }
        else if (!strcmp(argv[i], "-R"))
        {
            LSHData.radius = atoi(argv[i + 1]);
            cout << LSHData.radius << endl;
        }
        else if (!strcmp(argv[i], "--dist-true=visible"))
        {
            distance_true_visible = 1;
        }

    if (LSHData.inputFileName.empty() || LSHData.outputFileName.empty() || LSHData.queryFileName.empty())
    {
        cerr << "Arguments must contain all input file, output file and query file. The rest of the arguments are optional"
             << endl;
        return EXIT_FAILURE;
    }
    fstream inputFile;
    inputFile.open(LSHData.inputFileName, ios::in);
    if (!inputFile.is_open())
    {
        cerr << "Could not open the file: '"
             << LSHData.inputFileName << "'"
             << endl;
        return EXIT_FAILURE;
    }
    vector<string> inputLines;
    string line;
    while (getline(inputFile, line))
    {
        inputLines.push_back(line);
    }

    vector<Point *> inputPoints;
    // for (const auto &i : inputLines)
    //     cout << i << endl;

    // std::ofstream out(outputFileName);
    // std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
    // std::cout.rdbuf(out.rdbuf()); //redirect std::cout to out.txt!
    int numOfPoints = inputLines.size();
    int dimension = 0;

    for (int i = 0; i < numOfPoints; i++)
    {
        // separate string by Tabs
        // pick every element from 2nd to endl
        // read point coordinates
        PointPtr currPoint = new Point;
        string word = "";
        dimension = 0;
        for (char x : inputLines[i])
        {
            if (x == ' ')
            {
                if (dimension)
                    currPoint->coords.push_back(atof(word.c_str()));
                else
                    currPoint->id = word;
                // cout << word << endl;
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
    LSHData.dimension = dimension;
    cout << "dim" << dimension;

    HashTables HashTablesObject(LSHData.intL, LSHData.numberOfHyperplanes, numOfPoints, LSHData.dimension);

    for (int i = 0; i < inputPoints.size(); i++)
    {
        //    cout << "Point" << i << endl;
        HashTablesObject.HashTables::InsertPoint(inputPoints[i]);
    }

    // HashTablesObject.HashTables::PrintHashTables();

    fstream queryFile;
    queryFile.open(LSHData.queryFileName, ios::in);
    if (!queryFile.is_open())
    {
        cerr << "Could not open the file: '"
             << LSHData.queryFileName << "'"
             << endl;
        return EXIT_FAILURE;
    }
    vector<string> queryLines;
    // string line;
    while (getline(queryFile, line))
    {
        queryLines.push_back(line);
    }

    vector<PointPtr> queryPoints;

    dimension = 0;
    for (int i = 0; i < queryLines.size(); i++)
    {
        // separate string by Spaces
        // pick every element from 2nd to endl
        // read point coordinates
        PointPtr currPoint = new Point;
        string word = "";
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

    vector<kNeighboursPtr> queryTrueNeighbors;
    queryTrueNeighbors.resize(queryLines.size());

    vector<vector<PointPtr>> queryRangeSearch;
    queryRangeSearch.resize(queryLines.size());

    vector<double> tLSH;
    tLSH.resize(queryLines.size());
    vector<double> tTrue;
    tTrue.resize(queryLines.size());
    cout << "Starting methods" << endl;
    for (int i = 0; i < queryLines.size(); i++)
    {
        cout << i << endl;
        auto LSH_start = std::chrono::high_resolution_clock::now();
        queryOutputData[i] = HashTablesObject.HashTables::find_k_nearest_neighbours(queryPoints[i], LSHData.numberOfNearest);
        auto LSH_end = std::chrono::high_resolution_clock::now();
        tLSH[i] = std::chrono::duration_cast<std::chrono::milliseconds>(LSH_end - LSH_start).count();

        auto True_start = std::chrono::high_resolution_clock::now();
        queryTrueNeighbors[i] = find_k_true_neighbours(queryPoints[i], LSHData.numberOfNearest, inputPoints, LSHData.dimension);
        auto True_end = std::chrono::high_resolution_clock::now();
        tTrue[i] = std::chrono::duration_cast<std::chrono::milliseconds>(True_end - True_start).count();

        queryRangeSearch[i] = HashTablesObject.HashTables::range_search(queryPoints[i], LSHData.radius);
    }
    queryFile.close();
    // cout << "After kNeighbours" << endl;
    ofstream outputFile(LSHData.outputFileName);
    if (!outputFile.is_open())
    {
        cerr << "Could not open the file: '"
             << LSHData.outputFileName << "'"
             << endl;
        return EXIT_FAILURE;
    }

    for (int i = 0; i < queryLines.size(); i++)
    {
        outputFile << "Query: "
                   << queryPoints[i]->id << endl;

        for (int j = 0; j < queryOutputData[i]->size; j++)
        {
            outputFile << "Nearest neighbor-"
                       << j + 1 << ": " << queryOutputData[i]->neighbours[j]->point->id << endl
                       << "distanceLSH: " << queryOutputData[i]->neighbours[j]->dist << endl;
            if (distance_true_visible)
            {
                outputFile << "True Nearest neighbor-"
                           << j + 1 << ": " << queryTrueNeighbors[i]->neighbours[j]->point->id << endl;
            }
            outputFile << "distanceTrue: " << queryTrueNeighbors[i]->neighbours[j]->dist << endl;
        }

        outputFile << "tLSH: " << (double)(tLSH[i] / 1000) << 's' << endl
                   << "tTrue: " << (double)(tTrue[i] / 1000) << 's' << endl
                   << "R-near neighbors:" << endl;
        for (int j = 0; j < queryRangeSearch[i].size(); j++)
            outputFile << queryRangeSearch[i][j]->id << endl;

        outputFile << endl
                   << endl;
    }

    outputFile.close();
    return EXIT_SUCCESS;
}

// querypoints na einai vector apo PointPtr pou na exei ola ta poitns tous queryFile