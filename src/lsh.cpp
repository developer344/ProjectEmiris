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
    int intK = 4;
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
            intK = atoi(argv[i + 1]);
            cout << intK << endl;
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

    vector<Point *> points;
    // for (const auto &i : inputLines)
    //     cout << i << endl;

    // std::ofstream out(outputFileName);
    //std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
    // std::cout.rdbuf(out.rdbuf()); //redirect std::cout to out.txt!
    int numOfPoints = inputLines.size();
    int dimension = 0;

    for (int i = 0; i < numOfPoints; i++)
    {
        // separate string by Tabs
        // pick every element from 2nd to endl
        //read point coordinates
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

        points.push_back(currPoint);
    }
    dimension--;
    cout << "dim" << dimension;

    HashTables HashTablesObject(intL, intK, numOfPoints, dimension);

    cout << "OK" << endl;

    for (int i = 0; i < points.size(); i++)
    {
        cout << "Point" << i << endl;
        HashTablesObject.HashTables::InsertPoint(points[i]);
    }

    cout << "OK2" << endl;

    HashTablesObject.HashTables::PrintHashTables();

    // for (auto &i : points)
    // {
    //     cout << i.id << endl;
    //     for (auto &j : i.coords)
    //         cout << j << " ";
    //     cout << endl;
    // }

    inputFile.close();
    return EXIT_SUCCESS;
}