//
// Created by Haotian on 2017/8/21.
//

#include "AnalyzeTechnique.h"
#include <cmath>

double AnalyzeTechnique::CheckStorey(const Student &student, const Classroom &classroom) {
    switch (student.storey) {
        case 1:
            return (0.6 + classroom.storey / 10.0);
        case 2:
            return (1.0 - classroom.storey / 10.0);
        case 3:
            return (1.0 - (classroom.storey - 2) * (classroom.storey - 2) * 0.1);
        default:
            return 1.0;
    }
}

double AnalyzeTechnique::CheckPlug(const Student &student, const Classroom &classroom) {
    switch (student.plug) {
        case 1:
            if (classroom.plug == 0) { return 0.f;}
            return 1.f;
        case 2:
            if (classroom.plug == 0) { return 0.6; }
            return 1.f;
        case 3:
            if (classroom.plug == 0) { return 0.8; }
            return 1.f;
        default:
            return 1.f;
    }
}

double AnalyzeTechnique::CheckCapacity(const Student &student, const Classroom &classroom) {
    if (classroom.currentC > student.capacity) {
        return pow(0.8, student.stuEffect * getMax(0, classroom.currentC - student.capacity));
    }
    return 1.f;
}

double AnalyzeTechnique::CheckPart(const Student &student, const Classroom &classroom) {
    int stuPart;
    switch (classroom.part) {
        case 1:
            stuPart = student.preferB;
            break;
        case 2:
            stuPart = student.preferC;
            break;
        default:
            stuPart = student.preferA;
            break;
    }

    return (0.6 + (3 - stuPart) * 0.2);
}
