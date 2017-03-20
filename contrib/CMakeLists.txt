cmake_minimum_required(VERSION 3.1)
project (MatplotlibCPP_Test)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${PYTHONHOME}/include)
link_directories(${PYTHONHOME}/libs)

add_definitions(-DMATPLOTLIBCPP_PYTHON_HEADER=Python.h)

# message(STATUS "*** dump start cmake variables ***")
# get_cmake_property(_variableNames VARIABLES)
# foreach(_variableName ${_variableNames})
        # message(STATUS "${_variableName}=${${_variableName}}")
# endforeach()
# message(STATUS "*** dump end ***")

add_executable(minimal ${CMAKE_CURRENT_SOURCE_DIR}/../examples/minimal.cpp)
add_executable(basic ${CMAKE_CURRENT_SOURCE_DIR}/../examples/basic.cpp)
add_executable(modern ${CMAKE_CURRENT_SOURCE_DIR}/../examples/modern.cpp)
