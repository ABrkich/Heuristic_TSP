//
// Created by Andrew Brkich on 10/18/20.
//

#ifndef LAB_3_DYNAMIC_PROGRAMMING_ABRKICH_LOAD_H
#define LAB_3_DYNAMIC_PROGRAMMING_ABRKICH_LOAD_H

#include <tuple>
#include <vector>
#include <list>
#include <stdlib.h>

using namespace std;
class load{

private:
    vector<list<pair<int,double>>> adjList;
    vector<tuple<double, double, double>> positionVector;
public:
    load();
    ~load();
    vector<list<pair<int,double>>> loadGraphFile();
    vector<tuple<double, double, double>> loadPositionFile();
};

#endif //LAB_3_DYNAMIC_PROGRAMMING_ABRKICH_LOAD_H
