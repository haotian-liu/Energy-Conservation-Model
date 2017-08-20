//
// Created by Haotian on 2017/8/20.
//

#ifndef ENERGYCONSERVATIONMODEL_BASE_H
#define ENERGYCONSERVATIONMODEL_BASE_H

#include <string>

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

struct Res {
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

#endif //ENERGYCONSERVATIONMODEL_BASE_H
