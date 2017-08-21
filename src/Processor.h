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
    explicit AnalyzerResult(double SFTotal, double emission, double value) : SFTotal(SFTotal), emission(emission), value(value) {};
    double SFTotal;
    int foundCnt;
    int notFoundCnt;
    double emission;
    double value;
};

class Processor {
public:
    explicit Processor(Preprocessor *fp) : fp(fp) {};
    void init(const std::string &processFileName);
    bool rake();
    bool output(const std::string &outputFileName, const Config &config);
private:
    void GenList();
    bool Save(const std::string& processFileName) const;
    void PreProcess(std::vector<Student> &students);
    double PreProcess(std::vector<Classroom *> &classrooms, const std::vector<const CueItem *> &item);
    void GenCues(std::vector<const CueItem *> &GenItems, int k) const;
    void Process(std::vector<Student> &students, std::vector<Classroom *> &classrooms);
    void Analyze(std::vector<Student> &students);
    CueList CueListSampler;
    Preprocessor *fp;
    std::vector<struct Result> bestResults;
    std::vector<AnalyzerResult> sampler;
    double SFTotal;
    int foundCnt;
    int notFoundCnt;
    double emission;
    double average;
};


#endif //ENERGYCONSERVATIONMODEL_PROCESSOR_H
