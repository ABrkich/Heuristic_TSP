//
// Created by Andrew Brkich on 10/21/20.
//

#ifndef LAB_3_DYNAMIC_PROGRAMMING_ABRKICH_ALGORITHMS_H
#define LAB_3_DYNAMIC_PROGRAMMING_ABRKICH_ALGORITHMS_H
#include <tuple>
#include <vector>
#include <list>
using namespace std;
class algorithms{
private:
public:
    virtual pair<vector<int>,double> bruteForceTSP(vector<list<pair<int,double>>>& adjList, vector<tuple<double, double, double>>&, int& source) = 0;
    virtual pair<vector<int>, double> dynamicTSP(vector<list<pair<int,double>>>& adjList, vector<tuple<double, double, double>>&, int& source) = 0;
    virtual pair<vector<int>,double> simulatedAnnealingTSP(vector<list<pair<int,double>>>& adjList, vector<tuple<double, double, double>>&, int& source) = 0;
    virtual void compareShortest(vector<int> &possiblePath, vector<tuple<double, double, double>>&, double &, vector<int>&) = 0;
    virtual double calculateDistance(vector<tuple<double, double, double>>& positionVector, int& source, int& destination) = 0;
    virtual pair<vector<int>,double> geneticAlgorithmTSP(vector<list<pair<int,double>>>& adjList, vector<tuple<double, double, double>>&, int& source) = 0;
};
#endif //LAB_3_DYNAMIC_PROGRAMMING_ABRKICH_ALGORITHMS_H
