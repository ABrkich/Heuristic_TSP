//
// Created by Andrew Brkich on 10/21/20.
//
#include "tsp.h"
#include <tuple>
#include <vector>
#include <cmath>
#include <iostream>
#include <list>
#include <algorithm>
#include <map>
#include <random>
#include <stdlib.h>
using namespace std;

tsp::tsp() {}
tsp::~tsp() {}


double tsp::calculateDistance(vector<tuple<double, double, double>>& positionVector, int& source, int& destination) {
    double distance;
    double x1,x2,y1,y2,z1,z2;
    x1 = get<0>(positionVector[source]);
    y1 = get<1>(positionVector[source]);
    z1 = get<2>(positionVector[source]);
    x2 = get<0>(positionVector[destination]);
    y2 = get<1>(positionVector[destination]);
    z2 = get<2>(positionVector[destination]);
    //so because this method will be called a lot, I am leaving out the square root calculation
    //square roots are time consuming so we stick to just squaring the numbers here
    //we don't need to know the actual distance, we just want to know what's the shortest distance
    //this will NOT effect which path is the "shortest" path
    distance = pow((x2-x1),2)+pow((y2-y1),2)+pow((z2-z1),2);
    return distance;
}

//brute force algorithm for the Traveling Salesman Problem (find a hamiltonian circuit)
pair<vector<int>,double> tsp::bruteForceTSP(vector<list<pair<int,double>>>& adjList, vector<tuple<double, double, double>>& positionVector, int& source) {
    //references:
    //https://www.geeksforgeeks.org/travelling-salesman-problem-set-1/
    //https://www.hackerearth.com/practice/algorithms/graphs/hamiltonian-path/tutorial/

    //1: generate all possible paths, only choose valid paths even though permutations library generates non-valid paths
    vector<int> possiblePath;
    vector<vector<int>> paths;
    for (int i = 1; i < adjList.size(); i++)
    {
        possiblePath.push_back(i);
    }
    possiblePath.push_back(1);

    do
    {
        bool valid = true;
        for (int x = 0; x < possiblePath.size()-1; x++)
        {
            bool found_good_child = false;
            for (auto c = adjList[possiblePath[x]].begin(); c != adjList[possiblePath[x]].end(); c++)
            {
                if(c->first == possiblePath[x+1]){
                    found_good_child = true;
                    break;
                }
            }
            if (found_good_child == false)
            {
                valid = false;
                break;
            }
        }
        if (valid == true) {
            paths.push_back(possiblePath);
        }
    }
    while (next_permutation(possiblePath.begin()+1, possiblePath.end()-1 ));


    //2: choose the shortest path, by calculating the cost of each path we found
    vector<double> costs;
    for (int i = 0; i < paths.size(); i++)
    {
        double cost = 0;
        for (int j = 0; j < paths[i].size() -1; j++)
        {
            double distance = calculateDistance(positionVector, paths[i][j], paths[i][j+1]);
            cost += distance;
        }
        costs.push_back(cost);
    }

    double lowestCost = 999999999;
    int lowestIndex = 999999999;
    for (int i = 0; i < costs.size(); i++)
    {
        if (costs[i] < lowestCost)
        {
            lowestCost = costs[i];
            lowestIndex = i;
        }
    }

    return make_pair(paths[lowestIndex],lowestCost);

}

struct combinations
{
    //references
    //https://stackoverflow.com/questions/9430568/generating-combinations-in-c
    //https://www.geeksforgeeks.org/print-all-possible-combinations-of-r-elements-in-a-given-array-of-size-n/
    //https://stackoverflow.com/a/9432150

    int N, R;
    vector<int> curr;
    int combosGenerated;
    bool combosCompleted;
    combinations(int N, int R) :combosCompleted(N < 1 || R > N),combosGenerated(0),N(N), R(R)
    {
        for (int i = 1; i <= R; ++i)
            curr.push_back(i);
    }
    vector<int> next()
    {
        vector<int> returned = curr;
        combosCompleted = true;
        for (int i = R - 1; i >= 0; --i)
            if (curr[i] < N - R + i + 1)
            {
                int j = curr[i] + 1;
                while (i <= R-1)
                {
                    curr[i++] = j++;
                }
                combosCompleted = false;
                ++combosGenerated;
                break;
            }

        return returned;
    }
};


pair<vector<int>, double> tsp::dynamicTSP(vector<list<pair<int,double>>>& adjList, vector<tuple<double, double, double>>& positionVector, int& source)
{
    //references
    //https://en.wikipedia.org/wiki/Held–Karp_algorithm
    //https://stackoverflow.com/questions/34209375/c-implementation-of-held-karp-and-speed
    //mostly transferred from python code at link below
    //https://github.com/CarlEkerot/held-karp/blob/master/held-karp.py
    //big0(2^n) timing

    //create adjMatrix matrix
    vector<vector<bool>> adjMatrix;
    for (int i =0; i < adjList.size(); i++)
    {
        vector<bool> newVector;
        for(int j = 0; j < adjList.size(); j++)
        {
            newVector.push_back(false);
        }
        adjMatrix.push_back(newVector);
    }
    for (int i = 1; i < adjList.size(); i++)
    {
        list<pair<int,double>>::const_iterator it = adjList[i].begin();
        for (int j = 0; j < adjList[i].size(); j++)
        {
            adjMatrix[i][it->first] = true;
            it++;
        }
    }

    //use our adjMatrix to create a distance Matrix to use
    //traverse through it, every time we get a true, put the distance of that row and collumn
    //if we get a false add 99999
    vector<vector<double>> dMat;
    for (int i = 1; i < adjMatrix.size(); i++)
    {
        vector<double> newVector;
        for (int j = 1; j < adjMatrix.size(); j++)
        {
            if (adjMatrix[i][j] == true)
            {
                newVector.push_back(calculateDistance(positionVector, i, j));
            }
            else
                newVector.push_back(99999999);
        }
        dMat.push_back(newVector);
    }

    //# Maps each subset of the nodes to the cost to reach that subset, as well
    //# as what node it passed before reaching this subset.
    //# Node subsets are represented as set bits.
    map<pair<int,int>,pair<double,double>> theMap;

    // Set transition cost from initial state
    for (int i = 1; i < adjList.size(); i++)
    {
        pair<int, int> key = make_pair((1 << i), i);
        pair<double, double> value = make_pair((dMat[0][i]), 0);
        theMap[key] = value;
    }

    // Iterate subsets of increasing length and store intermediate results
    // in classic dynamic programming manner
    for(int i = 2; i < dMat.size(); i++)
    {
        vector<vector<int>> paths;
        combinations combos(dMat.size()-1, i);
        while (!combos.combosCompleted)
        {
            vector<int> temp = combos.next();
            paths.push_back(temp);
        }
        for(int Path = 0; Path < paths.size(); Path++)
        {
            vector<int> path = paths[Path];
            int bits = 0;
            for(auto x : path)
            {
                bits |= 1 << x;
            }

            // Find the lowest cost to get to this subset
            for(int j = 0; j < path.size(); j++)
            {
                int J = path[j];
                int previous = bits & ~(1 << J);

                vector<pair<double,int>> res;
                for(auto k : path)
                {
                    if(k == 0 || k == J)
                    {
                        continue;
                    }
                    pair<int,int> key = make_pair(previous,k);
                    pair<double,int> it = make_pair((theMap.find(key)->second.first+dMat[k][J]),k);
                    res.push_back(it);
                }
                pair<int,int> key = make_pair(bits,J);
                double min = 99999;
                int lowestIndex = 0;
                for(int i = 0; i < res.size(); i++)
                {
                    if(res[i].first < min)
                    {
                        min = res[i].first;
                        lowestIndex = i;
                    }
                }
                theMap[key] = res[lowestIndex];
            }
        }
    }

    // We're interested in all bits but the least significant (the start state)
    int bits = (pow(2,dMat.size()) - 1) - 1;

    // Calculate optimal cost
    vector<pair<double,int>> res;
    for(int k = 1; k < dMat.size(); k++)
    {
        pair<int,int> key = make_pair(bits,k);
        res.push_back(make_pair((theMap.find(key)->second.first+dMat[k][0]),k));
    }
    double min = 99999;
    int lowestIndex = 0;
    for(int i = 0; i < res.size(); i++)
    {
        if(res[i].first < min)
        {
            min = res[i].first;
            lowestIndex = i;
        }
    }
    pair<double,int> opt = res[lowestIndex];
    pair<double,int> parent = res[lowestIndex];

    // Backtrack to find full path
    vector<int> returnPath;
    for(int i = 0; i < dMat.size()-1; i++)
    {
        returnPath.push_back(parent.second);
        int new_bits = bits & ~(1 << parent.second);
        pair<int,int> key = make_pair(bits,parent.second);
        pair<double,int> und = theMap[key];
        parent = theMap[key];
        bits = new_bits;
    }

    // Add implicit start state
    returnPath.push_back(0);
    reverse(returnPath.begin(),returnPath.end());
    returnPath.push_back(0);
    for(int & i : returnPath) i++;
    return make_pair(returnPath,opt.first);
}


pair<vector<int>, double> tsp::simulatedAnnealingTSP(vector<list<pair<int, double>>> &adjList,vector<tuple<double, double, double>> &positionVector, int &source)
{
    //references
    //https://www.technical-recipes.com/2012/c-implementation-of-hill-climbing-and-simulated-annealing-applied-to-travelling-salesman-problems/
    //https://www.youtube.com/watch?v=1kgbwosVUPs
    //https://github.com/deerishi/tsp-using-simulated-annealing-c-/blob/master/tsp.cpp
    //https://www.codeproject.com/Articles/26758/Simulated-Annealing-Solving-the-Travelling-Salesma

    vector<int> shortestPath, currentPath, possiblePath;
    double leastCost = 99999999;
    //create initial path, then pop off source node at the end so it doesnt get lost in the shuffle, will re-push later
    for (int i = 1; i < adjList.size(); i++) {possiblePath.push_back(i);}
    possiblePath.push_back(1);
    shortestPath = possiblePath;
    possiblePath.pop_back();

    double initialTemp = 999, rateOfCooling  = .1, minTemp = .99, probabilityOfRandomness = 99, randomnessCooling = .01;;
    double currentTemp = initialTemp;

    //iterate until we cool down
    //rate of randomness will exponentially grow smaller and smaller, so we have lots of randomness in the beginning, and not a lot at the end
    while(currentTemp > minTemp)
    {
        //create a % probability for randomness that decreases as temp cools
        currentTemp -= rateOfCooling;
        probabilityOfRandomness -= randomnessCooling;
        if (probabilityOfRandomness >= 98)
            randomnessCooling += .001;

        //high randomness
        int randomize = (rand() % 101);
        if (randomize <= probabilityOfRandomness)
        {
            //random shuffle
            random_shuffle(possiblePath.begin()+1, possiblePath.end());
            compareShortest(possiblePath,positionVector,leastCost,shortestPath);
            possiblePath.pop_back();
        }
        //if false, simply rotate, swap, or reverse, check new path, and continue iterations
        //this continues searching the search space, but with small/lower randomness than a random shuffle
        else{
            //rotation
            int rotateBy = (rand() % 4) + 4;
            rotate(possiblePath.begin() +1, (possiblePath.begin() + rotateBy), possiblePath.end());
            compareShortest(possiblePath, positionVector, leastCost, shortestPath);
            //pop off source node at the end so it doesn't get lost in the shuffles
            possiblePath.pop_back();

            //random swap of two elements
            int id1 =(rand() % (possiblePath.size() -1) + 1);
            int id2 = (rand() % (possiblePath.size() -1) + 1);
            swap(possiblePath[id1], possiblePath[id2]);
            compareShortest(possiblePath,positionVector, leastCost, shortestPath);
            possiblePath.pop_back();

            //reversal
            reverse(possiblePath.begin() +1, possiblePath.end());
            compareShortest(possiblePath,positionVector,leastCost,shortestPath);
            possiblePath.pop_back();
        }
    }
    //return shortestPath here
    return make_pair(shortestPath,leastCost);
}

void tsp::compareShortest(vector<int> &possiblePath, vector<tuple<double, double, double>>&positionVector, double &leastCost, vector<int>& shortestPath)
{
    double distance = 0;
    //push source node to the end, creates a hamiltonian circuit
    possiblePath.push_back(possiblePath[0]);
    //calculate the distance of this path, compare it to our shortest path
    for (int i = 0; i < possiblePath.size() -1; i++)
    {
        distance += calculateDistance(positionVector,possiblePath[i], possiblePath[i+1]);
    }
    if (distance < leastCost)
    {
        leastCost = distance;
        shortestPath = possiblePath;
    }
}


pair<vector<int>,double> tsp::geneticAlgorithmTSP(vector<list<pair<int, double>>> &adjList,vector<tuple<double, double, double>> &positionVector, int &source)
{
    //references
    //https://www.geeksforgeeks.org/genetic-algorithms/
    //http://www.theprojectspot.com/tutorial-post/applying-a-genetic-algorithm-to-the-travelling-salesman-problem/5
    //https://towardsdatascience.com/evolution-of-a-salesman-a-complete-genetic-algorithm-tutorial-for-python-6fe5d2b3ca35

    vector<city> cityList;
    //load cityList here
    for (int i = 0; i < positionVector.size(); i++)
    {
        city newCity(i, get<0>(positionVector[i]), get<1>(positionVector[i]), get<2>(positionVector[i]));
        cityList.push_back(newCity);
    }

    int populationSize = adjList.size()-1;
    int generations = 5000;

    //define population
    Population pop = Population(cityList, populationSize);

    //define fittest path
    Tour bestTour = pop.getFittest();

    //define genetics
    Genetics genes;

    //breed and get our result
    for (int i = 0; i < generations; i++)
    {
        pop = genes.evolve(pop);
    }
    bestTour = pop.getFittest();

    //return bestTour as a path
    vector<int> path;
    for(auto i : bestTour.tour)
    {
        path.push_back(i.idNumber+1);
    }
    //push back source node to make a circuit
    path.push_back(bestTour.tour[0].idNumber+1);
    double cost = bestTour.getDistance();
    return make_pair(path,cost);
}