//
// Created by Andrew Brkich on 10/18/20.
//
#include "load.h"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <tuple>
#include <list>

load::load() {}
load::~load() {}

using namespace std;
//////edit graph size here
////36 is the max number for the graph file provided
int nodes_to_run_on = 10;

//function works on following format: (First item in each line is source node, each number following is a destination of that node. Delimited by commas)
//the graph file itself can be found in the 'Data' folder
vector<list<pair<int,double>>> load::loadGraphFile() {
    string file = "../Data/Graph/graph.txt";
    ifstream myFile;
    myFile.open(file);
    if (!myFile)
    {
        std::cout << "Error: could not open graph file" << std::endl;
    }
    //push a row of 0 to our adj list so that our indexes line up (i.e. adjList[1] is the first row not adjList[0])
    list<pair<int,double>> firstRow;
    adjList.push_back(firstRow);
    while (!myFile.eof())
    {
        char buffer[500] = "";
        myFile.getline(buffer, 500, '\n');
        char* curr = buffer;
        //empty line exception handling
        if (buffer[0] == '\0')
        {
            break;
        }
        list<pair<int,double>> newRow;
        //so because we lined up our indexes, we can skip the node # value which is the first number of each line of the graph.txt file
        while (*curr != ',')
        {
            curr++;
        }
        //skip the comma, so now we get to our destination data in the line
        curr++;
        //while curr does not equal the end of the line...
        while (*curr != '\0')
        {
            //get each destination item/node in that current line
            string str = "";
            while (*curr != ',' && *curr != '\0')
            {
                str += *curr;
                curr++;
            }
            //the 0 in make_pair is the weight/cost of the source->destination... for this project there are no weights
            //the pair format is kept for future expandability/use with weighted nodes
            newRow.push_back(make_pair(stoi(str), 0));
            curr++;
        }
        //and push that line (containing the possible destinations for that node) to our adjList
        //again, because we lined up our indexes, line 3 refers to the destinations for node #3
        adjList.push_back(newRow);
    }
    myFile.close();
  //  return adjList;

  //limit number of nodes here if preferred
  ///////////////////////////////
    vector<list<pair<int,double>>> testList;
    testList.push_back(firstRow);
    for (int i = 1; i < nodes_to_run_on; i++)
    {
        //testList.push_back(adjList[i]);
        list<pair<int,double>> newRow;
        for (auto j = adjList[i].begin(); j != adjList[i].end(); j++)
        {
            if (j->first < (nodes_to_run_on+1))
            {
                newRow.push_back(make_pair(j->first,0));
            }
        }
        testList.push_back(newRow);
    }
    return testList;
    ///////////////////////////////
}

//function works on following format: Node number followed by x,y,z coordinates. Delimited by commas
//the graph file itself can be found in the 'Data' folder
vector<tuple<double, double, double>> load::loadPositionFile() {
    string file = "../Data/Graph/positions.txt";
    ifstream myFile;
    myFile.open(file);
    if (!myFile)
    {
        std::cout << "Error: could not open graph file" << std::endl;
    }
    //push back an empty first row/first item so that the indexes of our nodes and line numbers line up correctly
    tuple<double, double, double> firstItem;
    positionVector.push_back(firstItem);
    while (!myFile.eof())
    {
        char buffer[500] = "";
        myFile.getline(buffer, 500, '\n');
        if (buffer[0] == '\0')
        {
            break;
        }
        char *curr = buffer;
        tuple <double, double, double> newTuple;
        //skip the first item in each row because, again, our indexes line up.
        while (*curr != ',')
        {
            curr++;
        }
        //skip the comma, so now we can start reading in our x,y,z coordinates
        curr++;
        double xValue,yValue,zValue;
        //while we don't reach the end of the line...
        while (*curr != '\0')
        {
            //get our X value
            string str = "";
            while (*curr != ',' && *curr != '\0')
            {
                str += *curr;
                curr++;
            }
            xValue = stod(str);
            curr++;
            str = "";

            //get our Y value
            while (*curr != ',' && *curr != '\0')
            {
                str += *curr;
                curr++;
            }
            yValue = stod(str);
            curr++;
            str = "";

            //get our Z Value
            while (*curr != ',' && *curr != '\0')
            {
                str += *curr;
                curr++;
            }
            zValue = stod(str);

            newTuple = make_tuple(xValue,yValue,zValue);
        }
        positionVector.push_back(newTuple);
    }
    myFile.close();
   // return positionVector;


    vector<tuple<double, double, double>> testVector;
    tuple<double, double, double> first;
    testVector.push_back(first);
    for (int i = 1; i < nodes_to_run_on; i++)
    {
        tuple<double, double, double> newTup;
        newTup = positionVector[i];
        testVector.push_back(newTup);
    }
    return testVector;

}
