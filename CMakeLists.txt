cmake_minimum_required(VERSION 4.0)
project(MLJetPack CXX)
include_directories(include)
add_executable(test_to_double
test/test_to_double.cpp
src/utils.cpp
)
add_executable(test_trim
test/test_trim.cpp
src/utils.cpp
)
add_executable(csv_validator
tools/csv_validator.cpp
src/utils.cpp
)