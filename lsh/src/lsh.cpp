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

using namespace std;

int main(int argc, char **argv)
{
    std::string option;
    while (!(option == "TERM" || option == "term"))
    {
        std::cout << std::endl
                  << std::endl
                  << "STARTING" << std::endl;

        // Getting arguments
        inputData *LSHData = getInputData(&argc, argv);
        if (LSHData == NULL)
            return EXIT_FAILURE;

        // Opening inputFile
        vector<std::string> inputLines = get_lines(LSHData->inputFileName);

        vector<PointPtr> inputPoints;
        LSHData->dimension = get_points(inputLines, &inputPoints);

        std::cout << "Dimension: " << LSHData->dimension;

        int numOfInputPoints = inputPoints.size();
        HashTables HashTablesObject(LSHData->intL, LSHData->numberOfHyperplanes, numOfInputPoints, LSHData->dimension, numOfInputPoints / 4);

        // Inserting points to hash table
        for (int i = 0; i < inputPoints.size(); i++)
            HashTablesObject.HashTables::InsertPoint(inputPoints[i]);

        // Getting lines from query file
        vector<std::string> queryLines = get_lines(LSHData->queryFileName);

        // Creating points from Lines
        vector<PointPtr> queryPoints;
        get_points(queryLines, &queryPoints);

        // LSH k nearest neighbor search
        std::cout << "Executing LSH search algorithm..." << std::endl;
        vector<vector<Neighbour> *> k_nearest_neighbours;
        k_nearest_neighbours.resize(queryLines.size());

        vector<kNeighboursPtr> queryOutputData;
        queryOutputData.resize(queryLines.size());

        vector<double> tLSH;
        tLSH.resize(queryLines.size());

        for (int i = 0; i < queryLines.size(); i++)
        {

            auto LSH_start = std::chrono::high_resolution_clock::now();
            queryOutputData[i] = HashTablesObject.HashTables::find_k_nearest_neighbours(queryPoints[i], LSHData->numberOfNearest);
            auto LSH_end = std::chrono::high_resolution_clock::now();
            tLSH[i] = std::chrono::duration_cast<std::chrono::milliseconds>(LSH_end - LSH_start).count();
        }

        // Brute force k nearest neighbor search
        std::cout << "Executing brute-force search algorithm..." << std::endl;

        vector<kNeighboursPtr> queryTrueNeighbors;
        queryTrueNeighbors.resize(queryLines.size());

        vector<double> tTrue;
        tTrue.resize(queryLines.size());

        for (int i = 0; i < queryLines.size(); i++)
        {
            auto True_start = std::chrono::high_resolution_clock::now();
            queryTrueNeighbors[i] = find_k_true_neighbours(queryPoints[i], LSHData->numberOfNearest, inputPoints, LSHData->dimension);
            auto True_end = std::chrono::high_resolution_clock::now();
            tTrue[i] = std::chrono::duration_cast<std::chrono::milliseconds>(True_end - True_start).count();
        }

        // LSH range search
        vector<vector<PointPtr>> queryRangeSearch;
        queryRangeSearch.resize(queryLines.size());

        std::cout << "Executing range search algorithm..." << std::endl;

        for (int i = 0; i < queryLines.size(); i++)
        {
            queryRangeSearch[i] = HashTablesObject.HashTables::range_search(queryPoints[i], LSHData->radius);
        }

        // Writing results to outputFile
        if (writeToOutput(LSHData, queryPoints, queryOutputData, queryTrueNeighbors, queryRangeSearch, tLSH, tTrue))
            return EXIT_FAILURE;

        // Deleting Data Structures
        deleteData(&inputPoints, &queryPoints, &k_nearest_neighbours, &queryOutputData, &queryTrueNeighbors);

        std::cout << "Rerun Program?..." << std::endl
                  << "======Options======" << std::endl
                  << "CONT to rerun" << std::endl
                  << "TERM to terminate" << std::endl
                  << "===================" << std::endl;
        while (true)
        {

            option = "";
            char ch;
            while ((ch = getchar()) != '\n')
                option += ch;
            if (option == "CONT" || option == "TERM" || option == "cont" || option == "term")
                break;
        }
    }
    return EXIT_SUCCESS;
}