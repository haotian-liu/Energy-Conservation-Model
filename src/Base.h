//
// Created by Haotian on 2017/8/20.
//

#ifndef ENERGYCONSERVATIONMODEL_BASE_H
#define ENERGYCONSERVATIONMODEL_BASE_H

#define getMax(a, b) ((a) < (b) ? (b) : (a))

#include <string>
#include <vector>

struct Student {
    int grade, gender;
    int storey, plug, capacity;
    int stuEffect;
    int preferA, preferB, preferC; // preference of different parts of buildings like A, B, and C.
    int generic;
    double satisfactory;
    std::string classroom;
    std::string inClassroom;
    bool isPlugged;
};

struct Result {
    int id;
    double satisfactory;
    int freeCapacity;
    double val;
};

struct Classroom {
    std::string id;
    int storey, plug, capacity, part;
    int currentC, currentP;
};

struct CueItem {
    CueItem(const char &building, const int &storey, const int &type)
            : building(building), storey(storey), type(type) {};
    char building;
    int storey;
    int type;
};

struct CueList {
    bool check() const { return studentCount != 0 && !items.empty(); }
    int studentCount;
    std::vector<CueItem> items;
};

#endif //ENERGYCONSERVATIONMODEL_BASE_H
