//
// Created by Andrew Brkich on 10/18/20.
//
#include <iostream>
#include "load.h"
#include "output.h"
#include <tuple>
#include <vector>
#include <list>
#include <chrono>
#include <random>
#include "tsp.h"
using namespace std;

int main(int argc, char* argv[])
{
    /////////
    /////to edit # of nodes run in a graph, go into and edit load.cpp, edit int nodes_to_run_on on line 16
    /////sample graph is provided in Graph Folder of Project (max 36 nodes)
    /////////

    srand(time(nullptr));

    int source = 1;
    load *loader = new load();
    output *save = new output();
    algorithms *algorithms = new tsp();

    vector<tuple<double, double, double>> positionVector = loader->loadPositionFile();
    vector<list<pair<int,double>>> adjList = loader->loadGraphFile();

    //loop through our algorithms and solve TSP
    using namespace chrono;
    for (int i = 0; i < 4; i++)
    {
        //brute force
        if (i == 0)
        {
            auto start = high_resolution_clock::now();
            pair<vector<int>, double> brute_force = algorithms->bruteForceTSP(adjList, positionVector, source);
            auto stop = high_resolution_clock::now();
            auto timing = duration_cast<microseconds>(stop - start);
            long duration = timing.count();
            vector<int> path = brute_force.first;
            double cost = brute_force.second;
            save->printStats(i, duration, path, cost);
            save->saveData(i, duration, path, cost);
        }
        //dynamic programming
        else if (i == 1)
        {
            auto start = high_resolution_clock::now();
            pair<vector<int>, double> dynamic_programming = algorithms->dynamicTSP(adjList, positionVector, source);
            auto stop = high_resolution_clock::now();
            auto timing = duration_cast<microseconds>(stop - start);
            long duration = timing.count();
            vector<int> path = dynamic_programming.first;
            double cost= dynamic_programming.second;
            save->printStats(i, duration, path, cost);
            save->saveData(i, duration, path, cost);
        }
        //simulated annealing
        else if (i ==2)
        {
            auto start = high_resolution_clock::now();
            pair<vector<int>,double> simulated_annealing = algorithms->simulatedAnnealingTSP(adjList,positionVector,source);
            auto stop = high_resolution_clock::now();
            auto timing = duration_cast<microseconds>(stop - start);
            long duration = timing.count();
            vector<int> path = simulated_annealing.first;
            double cost = simulated_annealing.second;
            save->printStats(i, duration, path, cost);
            save->saveData(i, duration, path, cost);
        }
        //genetic algorithm
        else // (i == 3)
        {
            auto start = high_resolution_clock::now();
            pair<vector<int>,double> genetic_algorithm = algorithms->geneticAlgorithmTSP(adjList, positionVector, source);
            auto stop = high_resolution_clock::now();
            auto timing = duration_cast<microseconds>(stop - start);
            long duration = timing.count();
            vector<int> path = genetic_algorithm.first;
            double cost = genetic_algorithm.second;
            save->printStats(i, duration, path, cost);
            save->saveData(i,duration,path,cost);
        }
    }

    delete loader;
    delete save;
    delete algorithms;
}