//
// Created by Andrew Brkich on 10/20/20.
//

#ifndef LAB_3_DYNAMIC_PROGRAMMING_ABRKICH_OUTPUT_H
#define LAB_3_DYNAMIC_PROGRAMMING_ABRKICH_OUTPUT_H

#include <tuple>
#include <vector>
#include <list>
#include <stdlib.h>

using namespace std;
class output {
private:
public:
    output();
    ~output();
    void saveData(int algoType, long timing, vector<int> path, double cost);
    void printStats(int algoType, long timing, vector<int> path, double cost);
};
#endif //LAB_3_DYNAMIC_PROGRAMMING_ABRKICH_OUTPUT_H
