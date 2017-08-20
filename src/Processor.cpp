//
// Created by Haotian on 2017/8/20.
//

#include "Processor.h"
#include <iostream>
#include <fstream>
#include <utility>
#include <algorithm>
#include <random>

void Processor::init(const std::string &processFileName) {
    GenList();
    Save(processFileName);
}

void Processor::GenList() {
    cue.items.clear(); // reset
    cue.studentCount = 200; // set all students

    /****************** ALL valid options ******************/
    const std::vector<char> _ALL_buildings = {'A', 'B', 'C'};
    const std::vector<int> _ALL_storeys = {-1, 0, 1, 2, 3, 4};
    const std::vector<int> _ALL_types = {0, 1, 2};
    /****************** ALL valid options ******************/

    std::vector<char> buildings = {'A', 'C'};
    std::vector<int> storeys = {0, 1, 2};
    std::vector<int> types = {0};
    for (const char & building : buildings) {
        for (const int & storey : storeys) {
            if (building == 'B' && storey == 0) { continue; }
            if (building == 'C' && (storey == 0 || storey == 4)) { continue; }
            for (const int & type : types) {
                cue.items.emplace_back(building, storey, type);
            }
        }
    }
}

bool Processor::Save(const std::string &processFileName) const {
    std::ofstream finProcess;
    finProcess.open(processFileName);

    if (!cue.check()) {
        std::cerr << "CueList is empty, check failed." << std::endl;
        return false;
    }

    finProcess << cue.studentCount << std::endl;
    for (const auto & item : cue.items) {
        finProcess << item.building << " " << item.storey << " " << item.type;
        finProcess << std::endl;
    }
    return true;
}

bool Processor::rake() {
    if (!cue.check()) {
        std::cerr << "CueList is empty, check failed." << std::endl;
        return false;
    }

    auto size = cue.items.size();
    long long shifts = static_cast<long long>(1) << size;
    std::vector<const CueItem*> CueItems;

    for (int i = 0; i < shifts; i++) {
        notFoundCnt = 0;
        foundCnt = 0;
        SFTotal = 0;
        // generate cue items using binary digits
        GenCues(CueItems, i);

        auto students = fp->getStudents();
        PreProcess(students);

        std::vector<Classroom*> classrooms;
        PreProcess(classrooms, CueItems);

        Process(students, classrooms);

        Analyze(students);
    }
    return true;
}

void Processor::GenCues(std::vector<const CueItem *> &GenItems, int k) const {
    int p = 0;
    GenItems.clear();
    while (k > 0) {
        if (k % 2 == 1) { GenItems.push_back(&cue.items[p]); }
        k >>= 1;
        p++;
    }
}

void Processor::PreProcess(std::vector<Student> &students) {
    auto shuffled = fp->getStudents();
    auto engine = std::default_random_engine{};

    // map the students to desired numbers.
    std::shuffle(std::begin(shuffled), std::end(shuffled), engine);
    if (cue.studentCount > students.size()) {
        students.insert(students.end(), shuffled.begin(), shuffled.begin() + cue.studentCount - students.size());
    }

    // shuffle the student list
    std::shuffle(std::begin(students), std::end(students), engine);
}

int Processor::PreProcess(std::vector<Classroom *> &classrooms, const std::vector<const CueItem *> &items) {
    std::vector<Classroom> &classReference = fp->getClassroom();
    int freeCapacity = 0;

    for (const auto item : items) {
        for (auto & classroom : classReference) {
            // type (0,1,2) => (any, small, large)
            if ((classroom.id[0] == item->building)
                && (classroom.storey == item->storey || item->storey == -1)
                && (item->type == 0
                    || (item->type == 1 && classroom.capacity < 100)
                    || (item->type == 2 && classroom.capacity >= 100))) {
                freeCapacity += classroom.capacity;
                classroom.currentC = 0;
                classroom.currentP = 0;
                classrooms.push_back(&classroom);
            }
        }
    }
    return freeCapacity;
}

void Processor::Process(std::vector<Student> &students, std::vector<Classroom *> &classrooms) {
    Student student;
    Classroom classroom;

    double satisfactory, bestSF;
    int bestClassroom;
    int probability;

    for (int i = 0; i < students.size(); i++) {
        student = students[i];
        bestSF = 0;
        bestClassroom = -1;
        for (int j = 0; j < classrooms.size(); j++) {
            int stuPart;

            satisfactory = 1;
            classroom = *classrooms[j];

            if (classroom.capacity <= classroom.currentC)
                continue;

            // check storage
            switch (student.storey) {
                case 1:
                    satisfactory *= 0.6 + classroom.storey / 10.0;
                    break;
                case 2:
                    satisfactory *= 1.0 - classroom.storey / 10.0;
                    break;
                case 3:
                    satisfactory *= 1.0 - (classroom.storey - 2) * (classroom.storey - 2) * 0.1;
                    break;
                default:
                    break;
            }

            // check plug
            switch (student.plug) {
                case 1:
                    if (classroom.plug == 0) satisfactory = 0;
                    break;
                case 2:
                    if (classroom.plug == 0) satisfactory *= 0.6;
                    break;
                case 3:
                    if (classroom.plug == 0) satisfactory *= 0.8;
                    break;
                default:
                    break;
            }

            // check capacity
            if (classroom.currentC > student.capacity) {
                satisfactory *= pow(0.8, student.stuEffect * getMax(0, classroom.currentC - student.capacity));
            }

            // check part of the classroom
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

            satisfactory *= 0.6 + (3 - stuPart) * 0.2;

            // update best
            if (satisfactory > bestSF) {
                bestClassroom = j;
                bestSF = satisfactory;
            }
        }

        // check if best found and manipulate data.
        if (bestClassroom != -1) {
            classroom = *classrooms[bestClassroom];
            if (student.plug <= 3 && student.plug >= 1 && classroom.currentP < classroom.plug) {
                probability = rand() % 3 + 1;
                if (student.plug > probability) {
                    students[i].isPlugged = true;
                    classrooms[bestClassroom]->currentP++;
                }
            }
            students[i].satisfactory = bestSF;
            students[i].inClassroom = classroom.id;
            classrooms[bestClassroom]->currentC++;
            SFTotal += bestSF;
            foundCnt++;
        } else {
            notFoundCnt++;
        }
    }
}

void Processor::Analyze(std::vector<Student> &students) {
    Student student;
    Classroom classroom;

    average = SFTotal / students.size();
    double stdDeviation = 0;

    for (const auto &student : students) {
        stdDeviation += pow(average - student.satisfactory, 2);
    }

    stdDeviation /= students.size();
    stdDeviation = sqrt(stdDeviation);

    sampler.emplace_back(SFTotal, average);
}

bool Processor::output(const std::string &outputFileName) const {
    std::ofstream fout(outputFileName);
    if (fout.fail()) {
        std::cerr << "Fail to open student data file." << std::endl;
        return false;
    }
    for (const auto &s : sampler) {
        fout << s.SFTotal << " " << s.average << std::endl;
    }
    return true;
}
