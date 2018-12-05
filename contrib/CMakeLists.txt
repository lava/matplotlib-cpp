cmake_minimum_required(VERSION 3.7)
project (MatplotlibCPP_Test)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${PYTHONHOME}/include)
include_directories(${PYTHONHOME}/Lib/site-packages/numpy/core/include)
link_directories(${PYTHONHOME}/libs)

add_definitions(-DMATPLOTLIBCPP_PYTHON_HEADER=Python.h)

# message(STATUS "*** dump start cmake variables ***")
# get_cmake_property(_variableNames VARIABLES)
# foreach(_variableName ${_variableNames})
#         message(STATUS "${_variableName}=${${_variableName}}")
# endforeach()
# message(STATUS "*** dump end ***")

add_executable(minimal ${CMAKE_CURRENT_SOURCE_DIR}/../examples/minimal.cpp)
add_executable(basic ${CMAKE_CURRENT_SOURCE_DIR}/../examples/basic.cpp)
add_executable(modern ${CMAKE_CURRENT_SOURCE_DIR}/../examples/modern.cpp)
add_executable(animation ${CMAKE_CURRENT_SOURCE_DIR}/../examples/animation.cpp)
add_executable(nonblock ${CMAKE_CURRENT_SOURCE_DIR}/../examples/nonblock.cpp)
add_executable(xkcd ${CMAKE_CURRENT_SOURCE_DIR}/../examples/xkcd.cpp)
add_executable(bar ${CMAKE_CURRENT_SOURCE_DIR}/../examples/bar.cpp)
