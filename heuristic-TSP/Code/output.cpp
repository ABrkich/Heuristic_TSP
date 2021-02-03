//
// Created by Andrew Brkich on 10/20/20.
//
#include "output.h"
#include <iostream>
#include <fstream>
using namespace std;

output::output() {}
output::~output() {}

//saveData will save the data to an output file that is easy to copy/paste over to an excel file (delimited by commas) for evaluation/analysis
void output::saveData(int algoType, long timing, vector<int> path, double cost) {
    ofstream outputFile;
    outputFile.open("../Data/Graph/output.txt", std::ios::out | std::ios::app);
    if (!outputFile)
    {
        std::cout << "Error: could not output file" << std::endl;
    }
    char* algoName;
    //brute force algorithm
    if (algoType == 0)
    {
        algoName = "Brute Force";
    }
        //dynamic programming algorithm
    else if (algoType == 1)
    {
        algoName = "Dynamic Programming";
    }
    else if (algoType ==2)
    {
        algoName = "Simulated Annealing";
    }
    else
    {
        algoName = "Genetic Algorithm";
    }
    outputFile << algoName << ",";
    outputFile << timing << ",";
    outputFile << cost << endl;
}

void output::printStats(int algoType, long timing, vector<int> path, double cost) {
    char* algoName;
    //brute force algorithm
    if (algoType == 0)
    {
        algoName = "Traveling Salseman Problem - Brute Force Algorithm";
    }
    //dynamic programming algorithm
    else if (algoType == 1)
    {
        algoName = "Traveling Salseman Problem - Dynamic Programming Algorithm";
    }
    //simulated annealing algorithm
    else if (algoType == 2)
    {
        algoName = "Traveling Salseman Problem - Simulated Annealing Algorithm";
    }
    else if (algoType == 3)
    {
        algoName = "Traveling Salseman Problem - Genetic Algorithm";
    }
    std::cout << "Statistics for " << algoName <<": " << std::endl;
    std::cout << "Time Spent: " << timing << " microseconds " << std::endl;
    std::cout << "Path Returned: ";
    for (int i = 0; i < path.size(); i++)
    {
        cout << path[i] << " ";
    }
    cout << "\n";
    std::cout << "Cost of Path: " << cost << endl;
    cout << "\n";
}