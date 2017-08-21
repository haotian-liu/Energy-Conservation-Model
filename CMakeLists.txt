cmake_minimum_required(VERSION 3.8)
project(EnergyConservationModel)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp src/Base.h src/Processor.cpp src/Processor.h src/Preprocessor.cpp src/Preprocessor.h src/AnalyzeTechnique.cpp src/AnalyzeTechnique.h)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(EnergyConservationModel ${SOURCE_FILES})