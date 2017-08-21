//
// Created by Haotian on 2017/8/20.
//

#include <iostream>
#include <fstream>
#include <utility>
#include <algorithm>
#include <map>
#include <random>
#include "Processor.h"
#include "AnalyzeTechnique.h"

#include <chrono>

using namespace std::chrono;

void Processor::init(const std::string &processFileName) {
    GenList();
    Save(processFileName);
}

void Processor::GenList() {
    CueListSampler.items.clear(); // reset
    CueListSampler.studentCount = 1000; // set all students

    /****************** ALL valid options ******************/
    const std::vector<char> _ALL_buildings = {'A', 'B', 'C'};
    const std::vector<int> _ALL_storeys = {-1, 0, 1, 2, 3, 4};
    const std::vector<int> _ALL_types = {0, 1, 2};
    /****************** ALL valid options ******************/

    std::vector<char> buildings = {'A', 'B', 'C'};
    std::vector<int> storeys = {0, 1, 2, 3, 4};
    std::vector<int> types = {0};
    for (const char & building : buildings) {
        for (const int & storey : storeys) {
            if (building == 'B' && storey == 0) { continue; }
            if (building == 'C' && (storey == 0 || storey == 4)) { continue; }
            for (const int & type : types) {
                CueListSampler.items.emplace_back(building, storey, type);
            }
        }
    }
}

bool Processor::Save(const std::string &processFileName) const {
    std::ofstream finProcess;
    finProcess.open(processFileName);

    if (!CueListSampler.check()) {
        std::cerr << "CueList is empty, check failed." << std::endl;
        return false;
    }

    finProcess << CueListSampler.studentCount << std::endl;
    for (const auto & item : CueListSampler.items) {
        finProcess << item.building << " " << item.storey << " " << item.type;
        finProcess << std::endl;
    }
    return true;
}

bool Processor::rake() {
    if (!CueListSampler.check()) {
        std::cerr << "CueList is empty, check failed." << std::endl;
        return false;
    }

    auto size = CueListSampler.items.size();
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
        emission = PreProcess(classrooms, CueItems);

        Process(students, classrooms);

        Analyze(students);
    }
    return true;
}

void Processor::GenCues(std::vector<const CueItem *> &GenItems, int k) const {
    int p = 0;
    GenItems.clear();
    while (k > 0) {
        if (k % 2 == 1) { GenItems.push_back(&CueListSampler.items[p]); }
        k >>= 1;
        p++;
    }
}

void Processor::PreProcess(std::vector<Student> &students) {
    const auto &studentsRef = fp->getStudents();
    static std::default_random_engine engine(time(0));

    // map the students to desired numbers.
    if (CueListSampler.studentCount > students.size()) {
        while (CueListSampler.studentCount >= students.size() + studentsRef.size())
            students.insert(students.end(), studentsRef.begin(), studentsRef.end());
        students.insert(students.end(), studentsRef.begin(), studentsRef.begin() + CueListSampler.studentCount - students.size());
    }

    // shuffle the student list
    std::shuffle(std::begin(students), std::end(students), engine);
}

double Processor::PreProcess(std::vector<Classroom *> &classrooms, const std::vector<const CueItem *> &items) {
    std::vector<Classroom> &classReference = fp->getClassroom();

    for (const auto item : items) {
        for (auto & classroom : classReference) {
            // type (0,1,2) => (any, small, large)
            if ((classroom.id[0] == item->building)
                && (classroom.storey == item->storey || item->storey == -1)
                && (item->type == 0
                    || (item->type == 1 && classroom.capacity < 100)
                    || (item->type == 2 && classroom.capacity >= 100))) {
                classroom.currentC = 0;
                classroom.currentP = 0;
                classrooms.push_back(&classroom);
            }
        }
    }
    return AnalyzeTechnique::CalculateEmission(classrooms, items);
}

void Processor::Process(std::vector<Student> &students, std::vector<Classroom *> &classrooms) {
    static std::map<std::string, double (*)(const Student &, const Classroom &)> TechniqueMap = {
            {"CheckCapacity", AnalyzeTechnique::CheckCapacity},
            {"CheckPart", AnalyzeTechnique::CheckPart},
            {"CheckPlug", AnalyzeTechnique::CheckPlug},
            {"CheckStorey", AnalyzeTechnique::CheckStorey},
    };

    Classroom *bestClassroom;

    double currentSF, bestSF;
    int probability;

    for (auto &student : students) {
        bestSF = 0;
        bestClassroom = nullptr;
        for (auto &classroom : classrooms) {
            currentSF = 1;

            if (classroom->capacity <= classroom->currentC) { continue; }

            for (const auto &Technique : TechniqueMap) {
                currentSF *= Technique.second(student, *classroom);
                if (currentSF < bestSF) { break; }
            }

            // update best
            if (currentSF > bestSF) {
                bestClassroom = classroom;
                bestSF = currentSF;
            }
        }

        // check if best found and manipulate data.
        if (bestClassroom != nullptr) {
            if (student.plug <= 3 && student.plug >= 1 && bestClassroom->currentP < bestClassroom->plug) {
                probability = std::rand() % 3 + 1;
                if (student.plug > probability) {
                    student.isPlugged = true;
                    bestClassroom->currentP++;
                }
            }
            student.satisfactory = bestSF;
            student.inClassroom = bestClassroom->id;
            bestClassroom->currentC++;
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

    sampler.emplace_back(SFTotal, emission, SFTotal / emission);
}

bool Processor::output(const std::string &outputFileName, const Config &config) {
    std::ofstream fout(outputFileName);
    if (fout.fail()) {
        std::cerr << "Fail to open student data file." << std::endl;
        return false;
    }

    for (const auto &s : sampler) {
        if (s.emission > config.MaxEmission) { continue; }
        fout << s.SFTotal << " " << s.emission << " " << s.value << std::endl;
    }
    return true;
}
