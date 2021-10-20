#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fstream>
#include <vector>

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
    for (const auto &i : inputLines)
        cout << i << endl;

    inputFile.close();
    return EXIT_SUCCESS;
}