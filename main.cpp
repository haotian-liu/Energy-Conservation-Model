#include "src/Base.h"
#include "src/Processor.h"
#include "src/Preprocessor.h"

int main() {
    const std::string classFile = "resources/classroom.in";
    const std::string studentFile = "resources/student.in";
    auto *fp = new Preprocessor;
    fp->Init(classFile, studentFile);
    fp->Load();
    return 0;
}