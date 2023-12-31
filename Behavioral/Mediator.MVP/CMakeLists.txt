#----------------------------------------
# VCPKG integration
#----------------------------------------
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "")
endif()

if (DEFINED CMAKE_TOOLCHAIN_FILE)
  message(STATUS "Vcpkg integration script found: " ${CMAKE_TOOLCHAIN_FILE})
else()
  message(STATUS "Vcpkg integration script NOT found!")
endif()

#----------------------------------------
# Project
#----------------------------------------
get_filename_component(PROJECT_NAME_STR ${CMAKE_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${PROJECT_NAME_STR})

cmake_minimum_required(VERSION 3.8)
project(${PROJECT_NAME_STR}_tests)

#----------------------------------------
# Source/Headers/Executable
#----------------------------------------
aux_source_directory(. SRC_LIST)
file(GLOB HEADERS_LIST "*.h" "*.hpp")

add_executable(${PROJECT_NAME} ${SRC_LIST} ${HEADERS_LIST})

#----------------------------------------
# Compile options
#----------------------------------------
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

#----------------------------------------
# Libraries
#----------------------------------------
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)

find_package(Catch2 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Catch2::Catch2 Catch2::Catch2WithMain)

# find_package(Boost)
# target_link_libraries(${PROJECT_NAME} PRIVATE Boost::boost)
