//
// Created by Haotian on 2017/8/20.
//

#ifndef ENERGYCONSERVATIONMODEL_PREPROCESSOR_H
#define ENERGYCONSERVATIONMODEL_PREPROCESSOR_H

#include <fstream>
#include <vector>
#include "Base.h"

const int MaxToleranceCapacity = 100;

class Preprocessor {
public:
    Preprocessor() : initialized(false) {};
    ~Preprocessor() = default;
    bool Init(const std::string &classFileName, const std::string &studentFileName);
    bool Load();
    const std::vector<Student>& getStudents() const { return students; }
    std::vector<Classroom>& getClassroom() { return classrooms; }
private:
    bool LoadStudent();
    bool LoadClassroom();
    bool initialized;
    std::ifstream finClass, finStudent;
    std::vector<Student> students;
    std::vector<Classroom> classrooms;
};


#endif //ENERGYCONSERVATIONMODEL_PREPROCESSOR_H
