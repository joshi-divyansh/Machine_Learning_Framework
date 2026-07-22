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
add_executable(random_csv_generator
tools/random_csv_generator.cpp
)
add_executable(min_max_scaler
tools/min_max_scaler.cpp
src/utils.cpp
)
