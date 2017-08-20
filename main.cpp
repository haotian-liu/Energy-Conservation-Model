#include "src/Base.h"
#include "src/Preprocessor.h"
#include "src/Processor.h"

int main() {
    const std::string classFile = "resources/classroom.in";
    const std::string studentFile = "resources/student.in";
    const std::string processFile = "resources/process.out";
    const std::string resultFile = "resources/result.out";

    srand((unsigned int) time(NULL));

    auto *fp = new Preprocessor;
    fp->Init(classFile, studentFile);
    fp->Load();

    auto *pc = new Processor(fp);
    pc->init(processFile);
    pc->rake();
    pc->output(resultFile);
    return 0;
}