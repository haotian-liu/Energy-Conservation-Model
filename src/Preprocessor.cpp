//
// Created by Haotian on 2017/8/20.
//

#include "Preprocessor.h"
#include <iostream>

bool Preprocessor::Init(const std::string &classFileName, const std::string &studentFileName) {
    finClass.open(classFileName);
    if (finClass.fail()) {
        std::cerr << "Fail to open class data file." << std::endl;
        return false;
    }

    finStudent.open(studentFileName);
    if (finStudent.fail()) {
        std::cerr << "Fail to open student data file." << std::endl;
        return false;
    }

    initialized = true;
    return true;
}

bool Preprocessor::Load() {
    if (!initialized) {
        std::cerr << "Not initialized yet." << std::endl;
        return false;
    }
    if (!LoadStudent()) {
        std::cerr << "Fail to load student data." << std::endl;
        return false;
    }

    if (!LoadClassroom()) {
        std::cerr << "Fail to load student data." << std::endl;
        return false;
    }
    return true;
}

bool Preprocessor::LoadStudent() {
    Student student;

    int t, feel;
    int t1, t2, t3;
    while (!finStudent.eof()) {
        finStudent >> student.grade >> student.gender >> student.storey >> student.plug;
        feel = 0;
        for (int i = 0; i < 5; i++) {
            finStudent >> t;
            if (t) feel++;
        }
        finStudent >> t;
        if (t) feel = 0;
        student.stuEffect = feel;

        finStudent >> t1 >> t2 >> t3;
        if (t1 == -2) t1 = 3;
        if (t2 == -2) t2 = 3;
        if (t3 == -2) t3 = 3;

        student.preferA = t1;
        student.preferB = t2;
        student.preferC = t3;

        finStudent >> student.generic;
        finStudent >> t; // jump too much choice.
        finStudent >> t; // capacity
        if (t < 0) t = MaxToleranceCapacity;

        student.capacity = t;

        finStudent >> student.classroom;
        finStudent >> t; // skip unknown
        students.push_back(student);
    }
    return true;
}

bool Preprocessor::LoadClassroom() {
    Classroom classroom;
    while (!finClass.eof()) {
        finClass >> classroom.id >> classroom.capacity >> classroom.plug;
        classroom.part = classroom.id[0] - 'A';
        classroom.storey = classroom.id[1] - '0';
        classroom.currentC = classroom.currentP = 0;
        classrooms.push_back(classroom);
    }
    return true;
}
