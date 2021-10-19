#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define ENDL '\n'

using namespace std;

int main(int argc, char **argv)
{
    if (!(argc % 2) || argc > 17 || argc < 7)
    {
        perror("Incorrect format of arguments\n");
    }

    string inputFile = "";
    string queryFile = "";
    string outputFile = "";
    int intK = 14;
    int intM = 10;
    int numberOfNearest = 1;
    int radius = 10000;
    int probes = 2;

    for (int i = 0; i < argc; i++)
        if (!strcmp(argv[i], "-i"))
        {
            inputFile = string(argv[i + 1]);
            cout << inputFile << ENDL;
        }
        else if (!strcmp(argv[i], "-q"))
        {
            queryFile = string(argv[i + 1]);
            cout << queryFile << ENDL;
        }
        else if (!strcmp(argv[i], "-o"))
        {
            outputFile = string(argv[i + 1]);
            cout << outputFile << ENDL;
        }
        else if (!strcmp(argv[i], "-k"))
        {
            intK = atoi(argv[i + 1]);
            cout << intK << ENDL;
        }
        else if (!strcmp(argv[i], "-L"))
        {
            intM = atoi(argv[i + 1]);
            cout << intM << ENDL;
        }
        else if (!strcmp(argv[i], "-N"))
        {
            numberOfNearest = atoi(argv[i + 1]);
            cout << numberOfNearest << ENDL;
        }
        else if (!strcmp(argv[i], "-R"))
        {
            radius = atoi(argv[i + 1]);
            cout << radius << ENDL;
        }
        else if (!strcmp(argv[i], "-probes"))
        {
            probes = atoi(argv[i + 1]);
            cout << probes << ENDL;
        }

    if (inputFile.empty() || outputFile.empty() || queryFile.empty())
    {
        perror("Arguments must contain all input file, output file and query file. The rest of the arguments are optional\n");
        exit(1);
    }
}
