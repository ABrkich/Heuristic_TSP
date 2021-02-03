//
// Created by Andrew Brkich on 10/20/20.
//

#ifndef LAB_3_DYNAMIC_PROGRAMMING_ABRKICH_TSP_H
#define LAB_3_DYNAMIC_PROGRAMMING_ABRKICH_TSP_H
#include "algorithms.h"
#include <tuple>
#include <vector>
#include <list>
#include <string>
#include <unordered_set>
#include <algorithm>
#include <cmath>
#include <random>
using namespace std;
class tsp: public algorithms{

private:
public:
    tsp();
    ~tsp();
    pair<vector<int>,double> bruteForceTSP(vector<list<pair<int,double>>>& adjList, vector<tuple<double, double, double>>&, int& source);
    pair<vector<int>, double> dynamicTSP(vector<list<pair<int,double>>>& adjList, vector<tuple<double, double, double>>&, int& source) ;
    pair<vector<int>, double> simulatedAnnealingTSP(vector<list<pair<int,double>>>& adjList, vector<tuple<double, double, double>>&, int& source);
    void compareShortest(vector<int> &possiblePath, vector<tuple<double, double, double>>&, double &,vector<int>&);
    double calculateDistance(vector<tuple<double, double, double>>& positionVector, int& source, int& destination);
    pair<vector<int>,double> geneticAlgorithmTSP(vector<list<pair<int,double>>>& adjList, vector<tuple<double, double, double>>&, int& source);



    //////////////////
    ///////////separate structs used for genetic algorithm

    //references for structs
    //http://www.theprojectspot.com/tutorial-post/applying-a-genetic-algorithm-to-the-travelling-salesman-problem/5
    struct city
    {
        double x,y,z;
        int idNumber;
        city(int id, double a, double b, double c)
        {
            idNumber = id;
            x = a;
            y = b;
            z = c;
        }
    };

    struct Tour
    {
        vector<city> tour;
        int size = 0;
        double distance = 0;
        double fitness = 0;

        Tour(){};

        Tour(vector<city> cityList)
        {
            initiate(cityList);
            size = tour.size();
        }

        //similar to the calculateDistance method, leaves out square root
        double findDistance(city a, city b)
        {
            return (pow((a.x - b.x),2) + pow((a.y - b.y),2) + pow((a.z - b.z),2));
        }

        //randomizes our paths so that each individual path in our list is different from the next
        void initiate(vector<city> cityList)
        {
            tour = cityList;
            random_shuffle(tour.begin()+1, tour.end());
        }
        //fitness will be determined by node distance
        double getFitness()
        {
            double distance = getDistance();
            return (1/distance);
        }
        //find the total distance of our path
        double getDistance()
        {
            if(distance == 0)
            {
                int size = tour.size();
                for(int i = 0; i < size-1; i++)
                {
                    distance += findDistance(tour[i],tour[i+1]);
                }
                distance += findDistance(tour[0], tour[tour.size()-1]);
            }
            return distance;
        }

        int getTourSize()
        {
            size = tour.size();
            return size;
        }

        void setcity(int index, city theCity) {tour[index] = theCity;}

        void setcity(city theCity)
        {
            tour.push_back(theCity);
            size++;
        }
    };

    struct Population{
        vector<Tour> tourList;
        vector<city> cityList;
        int size = 0;

        Population(){};

        //initialize our list of possible paths, each one randomly generated
        Population(vector<city> list, int populationSize)
        {
            cityList = list;
            for(int i = 0; i < populationSize; i++)
            {
                tourList.push_back(cityList);
            }
            size = tourList.size();
        }

        void addTour(Tour tour)
        {
            tourList.push_back(tour);
            size++;
        }

        Tour getTour(int index) {return tourList[index];}

        //find the best possible path
        Tour getFittest()
        {
            Tour bestTour = tourList[0];
            for(int i = 1; i < size; i++)
            {
                if(bestTour.getFitness() < tourList[i].getFitness())
                {
                    bestTour = tourList[i];
                }
            }
            return bestTour;
        }
    };

    struct Genetics{
        //breeding will be determined by elitism
        bool elitism = true;
        double mutationRate = .075;
        int tournamentSize = 5;
        vector<Tour> tours;

        //creates a "tournament" of our paths, and selects the "winner", or best path possible
        //see tournament size above
        Tour tournament(Population pop)
        {
            Population tournamentPopulation;
            for(int i = 0; i < tournamentSize; i++)
            {
                int randomTour = rand() % pop.size;
                tournamentPopulation.addTour(pop.getTour(randomTour));
            }
            Tour winner = tournamentPopulation.getFittest();
            return winner;
        }

        //Tournament that excludes one specific tour
        Tour tournament(Population pop, Tour t)
        {
            Population tournamentPopulation;
            int size = t.size;
            string p1Tour = "";
            for(int i = 0; i < size; i++)
            {
                p1Tour += t.tour[i].idNumber;
            }
            int i = 0;
            while(i < tournamentSize)
            {
                int randomTour = rand() % (pop.size-1);
                for(int j = 0; j < size; j++)
                {
                    if(pop.getTour(randomTour).tour[j].idNumber != p1Tour[j])
                    {
                        tournamentPopulation.addTour(pop.getTour(randomTour));
                        i++;
                        break;
                    }
                }
            }
            return tournamentPopulation.getFittest();
        }

        //Mutation method, see mutation rate above
        //swaps two random cities
        void mutate(Tour t)
        {
            int tourSize = t.getTourSize();
            for(int i = 0; i < tourSize; i++)
            {
                double random = rand()/RAND_MAX;
                if(random <= mutationRate)
                {
                    int id = rand() % tourSize;
                    city city1 = t.tour[i];
                    city city2 = t.tour[id];
                    t.setcity(id, city1);
                    t.setcity(i, city2);
                }
            }
        }

        //breeding process
        Tour cross(Tour parent1, Tour parent2)
        {
            Tour child = Tour();
            int tSize = parent1.getTourSize();
            unordered_set<int> citiesOnTour;

            //split parent 1 into two halves by a random rate
            int split1 = 1+ rand() % (tSize/2);
            int split2 = rand() % (tSize/2);
            //index
            split2 = tSize-split2;

            for(int i = 0; i < split1; i++)
            {
                child.setcity(parent1.tour[i]);
                citiesOnTour.insert(parent1.tour[i].idNumber);
            }
            for(int i = split2; i < tSize; i++)
            {
                child.setcity(parent1.tour[i]);
                citiesOnTour.insert(parent1.tour[i].idNumber);
            }
            //Fill in missing cities using parent 2
            int index = 0;
            while(child.getTourSize() != tSize)
            {
                if(citiesOnTour.count(parent2.tour[index].idNumber) == 0)
                {
                    child.setcity(parent2.tour[index]);
                }
                index++;
            }
            return child;
        }

        //Evolves for one generation
        Population evolve(Population p)
        {
            int size = p.size;
            Population nextGen = Population();
            int eliteOffset = 0;
            if(elitism){//saves fittest individual
                eliteOffset = 1;
                nextGen.addTour(p.getFittest());
            }
            for(int i = eliteOffset; i < size; i++)
            {
                Tour parent1 = tournament(p);
                Tour parent2 = tournament(p, parent1);
                Tour child = cross(parent1, parent2);
                nextGen.addTour(child);
            }
            for(int i = eliteOffset; i < size; i++)
            {//Mutate next generation
                mutate(nextGen.getTour(i));
            }
            return nextGen;
        }
    };

    /////////////////////end structs


};
#endif //LAB_3_DYNAMIC_PROGRAMMING_ABRKICH_TSP_H
