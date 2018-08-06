cmake_minimum_required(VERSION 3.6)
project(untitled1)

set(CMAKE_C_STANDARD 99)
add_definitions("-mpopcnt")
set(SOURCE_FILES main.c CSP/CSP.h CSP/CSP.c FutoshikiReader.h FutoshikiReader.c CSP/CSPConstraint.c CSP/CSPVar.c CSP/CSPDomain.c Constraints/Constraints.c Constraints/Constraints.h Algorithms/Uncached.c Algorithms/Uncached.h Algorithms/Common.c Algorithms/Common.h Algorithms/Cached.c Algorithms/Cached.h Algorithms/EPPreprocess.c Algorithms/EPPreprocess.h Algorithms/Configs.c Algorithms/Configs.h)
add_executable(untitled1 ${SOURCE_FILES})
target_link_libraries(untitled1 m)