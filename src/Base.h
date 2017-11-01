//
// Created by Haotian on 2017/8/20.
//

#ifndef ENERGYCONSERVATIONMODEL_BASE_H
#define ENERGYCONSERVATIONMODEL_BASE_H

#define getMax(a, b) ((a) < (b) ? (b) : (a))

#include <string>
#include <vector>

struct Student {
    double storeyPrefer(int storey) const {
        switch (this->storey) {
            case 1:
                return storey + 1;
            case 2:
                return 5 - storey;
            case 3:
                if (storey == 3) { return 5; }
                if (storey == 2 || storey == 4) { return 3; }
                return 1;
            case 4:
            default:
                return 3;
        }
    }
    int grade, gender, storey, plug;
    int prefer[3]; // preference of different parts of buildings like A, B, and C.
    double capacity;
    double satisfactory;
    std::string inClassroom;
    bool isPlugged;
};

struct Config {
    Config() : MaxEmission(0) {};
    int MaxEmission;
};

struct Classroom {
    void initialize() {
        part = id[0] - 'A';
        storey = id[1] - '0';
        currentC = currentP = 0;
    }
    double currentCapacity() const {
        return currentC * 1.0 / capacity;
    }
    std::string id;
    int storey, plug, capacity, part;
    int currentC, currentP;
    int walts;
    double lightEmission, airEmission, combEmission;
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
