# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(E906Ana CXX)

file(GLOB          sources ${PROJECT_SOURCE_DIR}/*.cc ${PROJECT_SOURCE_DIR}/*.cxx)
file(GLOB     dist_headers ${PROJECT_SOURCE_DIR}/*.h)
file(GLOB non_dist_headers ${PROJECT_SOURCE_DIR}/LinkDef.h)
list(REMOVE_ITEM dist_headers ${non_dist_headers})

# ROOT dict generation
add_custom_command (
  OUTPUT E906Ana_Dict.cc
  COMMAND rootcint
  ARGS -f E906Ana_Dict.cc -c 
    -I${PROJECT_SOURCE_DIR} -I${ROOT_PREFIX}/include -I$ENV{KTRACKER_ROOT}/inc
    ${dist_headers} ${PROJECT_SOURCE_DIR}/LinkDef.h
  DEPENDS ${dist_headers} ${PROJECT_SOURCE_DIR}/LinkDef.h
)

# ROOT
find_program(ROOTCONF "root-config")
if(ROOTCONF)
  message("-- Detecting ROOT:    found at ${ROOTCONF}")
else()
  message(FATAL_ERROR "-- Detecting ROOT:    not found")
endif()
execute_process(COMMAND root-config --prefix OUTPUT_VARIABLE ROOT_PREFIX  OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND root-config --cflags OUTPUT_VARIABLE ROOT_CFLAGS  OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND root-config --libs   OUTPUT_VARIABLE ROOT_LINK    OUTPUT_STRIP_TRAILING_WHITESPACE)

#MYSQL
execute_process(COMMAND mysql_config --include OUTPUT_VARIABLE MYSQL_INCLUDE OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND mysql_config --libs OUTPUT_VARIABLE MYSQL_LINK OUTPUT_STRIP_TRAILING_WHITESPACE)
string(REPLACE "-I" "" MYSQL_INCLUDE ${MYSQL_INCLUDE})
include_directories(${MYSQL_INCLUDE})

#GEANT4

execute_process(COMMAND geant4-config --prefix OUTPUT_VARIABLE GEANT4_PREFIX OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND geant4-config --libs OUTPUT_VARIABLE GEANT4_LINK OUTPUT_STRIP_TRAILING_WHITESPACE)
include_directories(${GEANT4_PREFIX}/include/Geant4)
message("----Geant: ${GEANT4_PREFIX}/include/Geant4")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++0x ${ROOT_CFLAGS} -I$ENV{KTRACKER_ROOT}/inc")

add_library(E906Ana SHARED ${sources} E906Ana_Dict.cc)
target_link_libraries(E906Ana  ${ROOT_LINK} -L$ENV{KTRACKER_ROOT}/lib kTracker E906DataCatalog ${MYSQL_LINK} ${GEANT4_LINK} )
install(TARGETS E906Ana DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

install(FILES ${dist_headers} DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
