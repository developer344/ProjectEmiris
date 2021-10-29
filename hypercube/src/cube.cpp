#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fstream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <climits>

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
                LSHData.numberOfHyperplanes = stoi(word);
        }
    }
    return EXIT_SUCCESS;
}
