//
// Created by Haotian on 2017/8/21.
//

#include "AnalyzeTechnique.h"
#include <cmath>
#include <iostream>

double AnalyzeTechnique::CheckDensityPlugs(const Student &student, const Classroom &classroom) {
    double x1 = classroom.currentCapacity() - student.capacity; // density_x1
    double x2 = (5 - student.plug) * (classroom.plug - classroom.currentP); // plug_x2;

    return (-5.98044794606417 * x1) + (-0.640442727008415 * x2);
}

double AnalyzeTechnique::CheckStoreyPart(const Student &student, const Classroom &classroom) {
    double x1 = 4 - student.prefer[classroom.part]; // part
    double x2 = student.storeyPrefer(classroom.storey); // storey

    return (0.346497722079504 * x1) + (0.396068718135752 * x2);
}

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
//    if (classroom.currentC > student.capacity) {
//        return pow(0.8, student.stuEffect * getMax(0, classroom.currentC - student.capacity));
//    }
    return 1.f;
}

double AnalyzeTechnique::CheckPart(const Student &student, const Classroom &classroom) {
//    int stuPart;
//    switch (classroom.part) {
//        case 1:
//            stuPart = student.preferB;
//            break;
//        case 2:
//            stuPart = student.preferC;
//            break;
//        default:
//            stuPart = student.preferA;
//            break;
//    }
//
//    return (0.6 + (3 - stuPart) * 0.2);
}

double AnalyzeTechnique::CalculateEmission(const std::vector<Classroom *> &classrooms, const std::vector<const CueItem *> &CueList) {
    double emission = 0;
    int SecACnt = 0, SecAElev = 0, SecBCCnt = 0;
    for (const auto item : CueList) {
        switch (item->building) {
            case 'A':
                SecACnt++;
                if (item->storey == 0) { emission += std::get<2>(EmissionList.find("A_Corridor_0")->second); }
                else { emission += std::get<2>(EmissionList.find("A_Corridor")->second); }
                if (item->storey == 4 || item->storey == 5) { SecAElev++; }
                break;
            case 'B':
                SecBCCnt++;
                emission += std::get<2>(EmissionList.find("B_Corridor")->second);
                if (item->storey == 1) { emission += std::get<2>(EmissionList.find("B1")->second); }
                break;
            case 'C':
                SecBCCnt++;
                emission += std::get<2>(EmissionList.find("C_Corridor")->second);
                if (item->storey == 1) { emission += std::get<2>(EmissionList.find("C1")->second); }
                break;
            default:
                std::cerr << "Unknown storey id when calculating emission in AT." << std::endl;
                break;
        }
    }
    if (SecACnt == SecATotal) { emission += std::get<2>(EmissionList.find("A_AirCondition")->second); }
    if (SecBCCnt == SecBCTotal) { emission += std::get<2>(EmissionList.find("BC_AirCondition")->second); }
    if (SecAElev == SecAElevTotal) { emission += std::get<2>(EmissionList.find("A_Elevator")->second); }

    for (const auto classroom : classrooms) {
        emission += classroom->combEmission;
    }
    return emission;
}
