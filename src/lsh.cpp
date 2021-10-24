#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fstream>
#include <vector>
#include "hashTable.h"

using namespace std;

int main(int argc, char **argv)
{
    //./lsh–i<inputfile> –q<queryfile> –k<int> -L<int> -ο<outputfile> -Ν<numberofnearest> -R<radius>
    if (!(argc % 2) || argc > 15 || argc < 7)
    {
        cerr << "Incorrect format of arguments"
             << endl;
        return EXIT_FAILURE;
    }

    string inputFileName = "";
    string queryFileName = "";
    string outputFileName = "";
    int numberOfHyperplanes = 4; // numberOfHyperplanes
    int intL = 5;
    int numberOfNearest = 1;
    int radius = 10000;

    for (int i = 0; i < argc; i++)
        if (!strcmp(argv[i], "-i"))
        {
            inputFileName = string(argv[i + 1]);
            cout << inputFileName << endl;
        }
        else if (!strcmp(argv[i], "-q"))
        {
            queryFileName = string(argv[i + 1]);
            cout << queryFileName << endl;
        }
        else if (!strcmp(argv[i], "-o"))
        {
            outputFileName = string(argv[i + 1]);
            cout << outputFileName << endl;
        }
        else if (!strcmp(argv[i], "-k"))
        {
            numberOfHyperplanes = atoi(argv[i + 1]);
            cout << numberOfHyperplanes << endl;
        }
        else if (!strcmp(argv[i], "-L"))
        {
            intL = atoi(argv[i + 1]);
            cout << intL << endl;
        }
        else if (!strcmp(argv[i], "-N"))
        {
            numberOfNearest = atoi(argv[i + 1]);
            cout << numberOfNearest << endl;
        }
        else if (!strcmp(argv[i], "-R"))
        {
            radius = atoi(argv[i + 1]);
            cout << radius << endl;
        }

    if (inputFileName.empty() || outputFileName.empty() || queryFileName.empty())
    {
        cerr << "Arguments must contain all input file, output file and query file. The rest of the arguments are optional"
             << endl;
        return EXIT_FAILURE;
    }
    fstream inputFile;
    inputFile.open(inputFileName, ios::in);
    if (!inputFile.is_open())
    {
        cerr << "Could not open the file: '"
             << inputFileName << "'"
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
    cout << "dim" << dimension;

    HashTables HashTablesObject(intL, numberOfHyperplanes, numOfPoints, dimension);

    for (int i = 0; i < inputPoints.size(); i++)
    {
        //    cout << "Point" << i << endl;
        HashTablesObject.HashTables::InsertPoint(inputPoints[i]);
    }

    // HashTablesObject.HashTables::PrintHashTables();

    fstream queryFile;
    queryFile.open(queryFileName, ios::in);
    if (!queryFile.is_open())
    {
        cerr << "Could not open the file: '"
             << queryFileName << "'"
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

    for (int i = 0; i < queryLines.size(); i++)
    {
        // separate string by Spaces
        // pick every element from 2nd to endl
        // read point coordinates
        PointPtr currPoint = new Point;
        string word = "";
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

    // vector<vector<Neighbour> *> k_nearest_neighbours;
    // k_nearest_neighbours.resize(queryLines.size());

    vector<kNeighboursPtr> queryOutputData;
    queryOutputData.resize(queryLines.size());

    for (int i = 0; i < queryLines.size(); i++)
    {
        queryOutputData[i] = HashTablesObject.HashTables::find_k_nearest_neighbours(queryPoints[i], numberOfNearest);
    }
    queryFile.close();
    cout << "After kNeighbours" << endl;
    ofstream outputFile(outputFileName);
    if (!outputFile.is_open())
    {
        cerr << "Could not open the file: '"
             << outputFileName << "'"
             << endl;
        return EXIT_FAILURE;
    }

    for (int i = 0; i < queryLines.size(); i++)
    {
        outputFile << "Query: "
                   << i << "\n";

        for (int j = 0; j < queryOutputData[i]->size; j++)
        {
            outputFile << "Nearest-neighbour-"
                       << j << ": " << queryOutputData[i]->neighbours[j]->point->id
                       << "\ndistanceLSH: " << queryOutputData[i]->neighbours[j]->dist
                       << "\ndistanceTrue: IDKLOL\n";
        }
    }

    outputFile.close();
    return EXIT_SUCCESS;
}

// querypoints na einai vector apo PointPtr pou na exei ola ta poitns tous queryFile