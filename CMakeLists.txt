cmake_minimum_required(VERSION 3.4...3.18)
project(dbow2py)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV REQUIRED)
find_package(pybind11 REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/DBoW2)

add_subdirectory(DBoW2)  # Make sure DBoW2 is cloned here or adjust path

pybind11_add_module(dbow2py bindings/pybind_interface.cpp)
target_link_libraries(dbow2py PRIVATE ${OpenCV_LIBS} DBoW2)

add_executable(test_vocab test_vocab.cpp)
target_link_libraries(test_vocab DBoW2 ${OpenCV_LIBS})
