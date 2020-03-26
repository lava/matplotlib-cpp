# From this directory:
# *WARNING* if you use a virtual python environment, you must have activated it before.
# ./configure
# then follow instructions
# cmake -S . -B /tmp --build doit aussi fonctionner

cmake_minimum_required(VERSION 3.15)

project(MatplotlibcppTests
        DESCRIPTION "matplotlibcpp unit tests"
        LANGUAGES CXX)

if(${USE_VIRTUEL_ENV})
    set(ENV{PATH} "$ENV{VIRTUAL_ENV}/bin:$ENV{PATH}")
endif()

set(PROJECT_VERSION_MAJOR  0)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 0)
set(PROJECT_VERSION_TWEAK 1)
set(PROJECT_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}.${PROJECT_VERSION_TWEAK})

# -----------------------------------------------
# -- CMake's variables
set(CMAKE_VERBOSE_MAKEFILE ON CACHE BOOL "Verbose Makefile" FORCE)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# -----------------------------------------------
# -- Catch
set(CATCH_MAIN ON)
option(BUILD_TESTING "Build nemesis-cpp testing" ${CATCH_MAIN}) 

# Prepare "Catch" library for other executables
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${PROJECT_SOURCE_DIR})

# -----------------------------------------------
# -- Python
set(PYTHON_VERSION "3.7")
find_package (Python3 COMPONENTS Interpreter Development)
# Valable pour un environnement python dans un python, dans un environnement virtuel ou dans un env spack,
set(SITE_PACKAGES "${Python3_SITELIB}")
set(NUMPY_INCLUDE_DIR "${SITE_PACKAGES}/numpy/core/include/numpy")
set(NUMPY_LIB_DIR "${SITE_PACKAGES}/numpy/core")

# -----------------------------------------------
# -- the project
set(PRJ_FILES "./tu_main.cpp;./tu_matplotlibcpp.cpp")
message(STATUS "PRJ_FILES = ${PRJ_FILES}")
set(TU_EXE "run_${PROJECT_NAME}")

set(MPLCPP_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/../../")
message(STATUS "MPLCPP_INCLUDE = ${MPLCPP_INCLUDE}")
# -----------------------------------------------
# -- rpath ...

# If your compiler is not in a standard path of your OS
if(${USE_CUSTOM_COMPILER})
    execute_process(COMMAND bash "-c" "LANG=C ${CMAKE_CXX_COMPILER} -print-search-dirs | sed -n 's/libraries: =//gp' | tr -s ':' ';'"
    OUTPUT_VARIABLE GNU_LIBRARIES_SEARCH_DIR)

    find_library(STDCPP_LIB stdc++ PATHS ${GNU_LIBRARIES_SEARCH_DIR})
    get_filename_component(_COMPILER_LIB_DIR ${STDCPP_LIB} DIRECTORY)
    set(COMPILER_RPATH_FLAGS "-Wl,-rpath,${_COMPILER_LIB_DIR}")
endif()

# -----------------------------------------------
# -- build executable
add_executable(${TU_EXE} ${PRJ_FILES})
add_test(NAME matplotlibcpp_tests
         COMMAND ${TU_EXE})

target_compile_features(${TU_EXE}
        PRIVATE
            cxx_std_17
            )
target_compile_definitions(${TU_EXE}
        PRIVATE
            CATCH_UNIT_TESTS)
target_compile_options(${TU_EXE}
            PRIVATE
            -fconcepts
            )
target_include_directories(${TU_EXE}
        PRIVATE
            ${Python3_INCLUDE_DIRS}
            ${NUMPY_INCLUDE_DIR}
            ${MPLCPP_INCLUDE}
            ${CMAKE_BINARY_DIR}
            )
target_link_libraries(${TU_EXE}
        PRIVATE
            ${Python3_LIBRARIES}
            -lstdc++fs
            )
target_link_options(${TU_EXE}
        PRIVATE
            ${COMPILER_RPATH_FLAGS}
            )

