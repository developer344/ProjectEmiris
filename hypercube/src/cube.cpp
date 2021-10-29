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
    while (!(option == "TERM"))
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
        ifstream inputFile;
        inputFile.open(HCData.inputFileName, ios::in);
        if (!inputFile.is_open())
        {
            cerr << "Could not open the file: '"
                 << HCData.inputFileName << "'"
                 << std::endl;
            return EXIT_FAILURE;
        }
        cout << "Reading input file " << HCData.inputFileName << "..." << endl;
        vector<std::string> inputLines;
        std::string line;
        while (getline(inputFile, line))
        {
            inputLines.push_back(line);
        }

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
        inputFile.close();
        dimension--;
        HCData.dimension = dimension;
        std::cout << "dim" << dimension;
        // Here insert points to Hypercube data structure when u figure out how
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
            if (option == "CONT" || option == "TERM")
                break;
        }
    }
    return EXIT_SUCCESS;
}
