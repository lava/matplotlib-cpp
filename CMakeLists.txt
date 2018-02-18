cmake_minimum_required(VERSION 3.0)

find_package(PythonLibs REQUIRED)

add_library(matplotlibcpp INTERFACE)
target_include_directories(matplotlibcpp INTERFACE include)
target_include_directories(matplotlibcpp INTERFACE ${PYTHON_INCLUDE_DIRS})
target_link_libraries(matplotlibcpp INTERFACE ${PYTHON_LIBRARIES})

option(MATPLOTLIBCPP_EXAMPLES "Build matplotlib examples" OFF)

if(MATPLOTLIBCPP_EXAMPLES)
    set (CMAKE_CXX_STANDARD 11)

    add_executable(minimal "examples/minimal.cpp")
    target_link_libraries(minimal matplotlibcpp)
    target_compile_definitions(minimal PRIVATE "-DWITHOUT_NUMPY")

    add_executable(basic "examples/basic.cpp")
    target_link_libraries(basic matplotlibcpp)

    add_executable(modern "examples/modern.cpp")
    target_link_libraries(modern matplotlibcpp)

    add_executable(animation "examples/animation.cpp")
    target_link_libraries(animation matplotlibcpp)

    add_executable(nonblock "examples/nonblock.cpp")
    target_link_libraries(nonblock matplotlibcpp)

    add_executable(xkcd "examples/xkcd.cpp")
    target_link_libraries(xkcd matplotlibcpp)

endif(MATPLOTLIBCPP_EXAMPLES)
