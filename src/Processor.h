//
// Created by Haotian on 2017/8/20.
//

#ifndef ENERGYCONSERVATIONMODEL_PROCESSOR_H
#define ENERGYCONSERVATIONMODEL_PROCESSOR_H

#include "Base.h"
#include "Preprocessor.h"
#include <vector>

struct AnalyzerResult {
    AnalyzerResult() = default;
    explicit AnalyzerResult(double SFTotal, double average, double value) : SFTotal(SFTotal), average(average), value(value) {};
    double SFTotal;
    int foundCnt;
    int notFoundCnt;
    int freeCapacity;
    double average;
    double value;
};

class Processor {
public:
    explicit Processor(Preprocessor *fp) : fp(fp) {};
    void init(const std::string &processFileName);
    bool rake();
    bool output(const std::string &outputFileName);
private:
    void GenList();
    bool Save(const std::string& processFileName) const;
    void PreProcess(std::vector<Student> &students);
    int PreProcess(std::vector<Classroom *> &classrooms, const std::vector<const CueItem *> &item);
    void GenCues(std::vector<const CueItem *> &GenItems, int k) const;
    void Process(std::vector<Student> &students, std::vector<Classroom *> &classrooms);
    void Analyze(std::vector<Student> &students);
    CueList cue;
    Preprocessor *fp;
    std::vector<struct Result> bestResults;
    std::vector<AnalyzerResult> sampler;
    double SFTotal;
    int foundCnt;
    int notFoundCnt;
    int freeCapacity;
    double average;
};


#endif //ENERGYCONSERVATIONMODEL_PROCESSOR_H
