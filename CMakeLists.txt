cmake_minimum_required(VERSION 3.15)
project(untitled)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-m64 -w -g -Wall -m64")

add_executable(untitled data.cpp GRASP.cpp ModelQuantity.cpp SubProblem.cpp main.cpp )
link_directories(C:\\xpressmp\\include C:\\xpressmp\\lib)
include_directories(C:\\xpressmp\\include C:\\xpressmp\\lib)

link_libraries(xprs.lib)
add_library(xprs SHARED IMPORTED)
link_libraries(xprb.lib)
add_library(xprb SHARED IMPORTED)
link_libraries(xprm_mc.lib)
add_library(xprm_mc SHARED IMPORTED)
link_libraries(xprm_rt.lib)
add_library(xprm_rt SHARED IMPORTED)

target_link_libraries(untitled "C:/xpressmp/lib/xprs.lib")

