cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(matplotlib_cpp LANGUAGES CXX)

include(GNUInstallDirs)
set(PACKAGE_NAME matplotlib_cpp)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/${PACKAGE_NAME}/cmake)


# Library target
add_library(matplotlib_cpp INTERFACE)
target_include_directories(matplotlib_cpp
  INTERFACE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/examples>
    $<INSTALL_INTERFACE:include>
)
target_compile_features(matplotlib_cpp INTERFACE
  cxx_std_11
)
# TODO: Use `Development.Embed` component when requiring cmake >= 3.18
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
target_link_libraries(matplotlib_cpp INTERFACE
  Python3::Python
  Python3::Module
)
find_package(Python3 COMPONENTS NumPy)
if(Python3_NumPy_FOUND)
  target_link_libraries(matplotlib_cpp INTERFACE
    Python3::NumPy
  )
else()
  target_compile_definitions(matplotlib_cpp INTERFACE WITHOUT_NUMPY)
endif()
install(
  TARGETS matplotlib_cpp
  EXPORT install_targets
)


# Examples
add_executable(minimal examples/minimal.cpp)
target_link_libraries(minimal PRIVATE matplotlib_cpp)
set_target_properties(minimal PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

add_executable(basic examples/basic.cpp)
target_link_libraries(basic PRIVATE matplotlib_cpp)
set_target_properties(basic PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

add_executable(modern examples/modern.cpp)
target_link_libraries(modern PRIVATE matplotlib_cpp)
set_target_properties(modern PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

add_executable(animation examples/animation.cpp)
target_link_libraries(animation PRIVATE matplotlib_cpp)
set_target_properties(animation PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

add_executable(nonblock examples/nonblock.cpp)
target_link_libraries(nonblock PRIVATE matplotlib_cpp)
set_target_properties(nonblock PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

add_executable(xkcd examples/xkcd.cpp)
target_link_libraries(xkcd PRIVATE matplotlib_cpp)
set_target_properties(xkcd PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

add_executable(bar examples/bar.cpp)
target_link_libraries(bar PRIVATE matplotlib_cpp)
set_target_properties(bar PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

add_executable(fill_inbetween examples/fill_inbetween.cpp)
target_link_libraries(fill_inbetween PRIVATE matplotlib_cpp)
set_target_properties(fill_inbetween PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

add_executable(fill examples/fill.cpp)
target_link_libraries(fill PRIVATE matplotlib_cpp)
set_target_properties(fill PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

add_executable(update examples/update.cpp)
target_link_libraries(update PRIVATE matplotlib_cpp)
set_target_properties(update PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

add_executable(subplot2grid examples/subplot2grid.cpp)
target_link_libraries(subplot2grid PRIVATE matplotlib_cpp)
set_target_properties(subplot2grid PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

add_executable(lines3d examples/lines3d.cpp)
target_link_libraries(lines3d PRIVATE matplotlib_cpp)
set_target_properties(lines3d PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

if(Python3_NumPy_FOUND)
  add_executable(surface examples/surface.cpp)
  target_link_libraries(surface PRIVATE matplotlib_cpp)
  set_target_properties(surface PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

  add_executable(colorbar examples/colorbar.cpp)
  target_link_libraries(colorbar PRIVATE matplotlib_cpp)
  set_target_properties(colorbar PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
  add_executable(contour examples/contour.cpp)
  target_link_libraries(contour PRIVATE matplotlib_cpp)
  set_target_properties(contour PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

  add_executable(spy examples/spy.cpp)
  target_link_libraries(spy PRIVATE matplotlib_cpp)
  set_target_properties(spy PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
endif()


# Install headers
install(FILES
  "${PROJECT_SOURCE_DIR}/matplotlibcpp.h"
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})


# Install targets file
install(EXPORT install_targets
  FILE
    ${PACKAGE_NAME}Targets.cmake
  NAMESPACE
    ${PACKAGE_NAME}::
  DESTINATION
    ${INSTALL_CONFIGDIR}
)


# Install matplotlib_cppConfig.cmake
include(CMakePackageConfigHelpers)
configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PACKAGE_NAME}Config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/${PACKAGE_NAME}Config.cmake
  INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)
install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/${PACKAGE_NAME}Config.cmake
  DESTINATION ${INSTALL_CONFIGDIR}
)
