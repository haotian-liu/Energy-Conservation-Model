#include "src/Base.h"
#include "src/Preprocessor.h"
#include "src/Processor.h"
#include <iostream>

int main(int argc, char **argv) {
    const std::string classFile = "resources/classroom.in";
    const std::string studentFile = "resources/student.in";
    const std::string processFile = "resources/process.out";
    const std::string resultFile = "resources/result.out";

    Config config;

    if (argc > 1) { config.MaxEmission = std::atoi(argv[1]); }

    if (config.MaxEmission == 0) { config.MaxEmission = INT32_MAX; }

    std::srand(std::time(0));

    auto *fp = new Preprocessor;
    if (!fp->Init(classFile, studentFile)) {
        std::cerr << "Fail to init" << std::endl;
        return -1;
    }
    if (!fp->Load()) {
        std::cerr << "Fail to load" << std::endl;
        return -1;
    }

    auto *pc = new Processor(fp);
    pc->init(processFile);
    pc->rake();
    pc->output(resultFile, config);
    return 0;
}