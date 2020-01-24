cmake_minimum_required(VERSION 3.15)
project(untitled)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-m64 -w -g -Wall -m64")



IF(WIN32)
    add_executable(untitled Data.cpp GRASP.cpp ModelQuantity.cpp SubProblem.cpp  Solution.cpp FGenetic.cpp main.cpp )
    link_directories(C:\\xpressmp\\include C:\\xpressmp\\lib)
    include_directories(C:\\xpressmp\\include C:\\xpressmp\\lib)
    target_link_libraries(untitled "C:/xpressmp/lib/xprs.lib")
ELSE(WIN32)
    SET (PROJECT_SOURCE_DIR /home-isis/auto/dapi/s18theve/untiltled)
    add_executable(./untitled  ${PROJECT_SOURCE_DIR}/Data.cpp  ${PROJECT_SOURCE_DIR}/GRASP.cpp  ${PROJECT_SOURCE_DIR}/ModelQuantity.cpp  ${PROJECT_SOURCE_DIR}/SubProblem.cpp   ${PROJECT_SOURCE_DIR}/Solution.cpp  ${PROJECT_SOURCE_DIR}/FGenetic.cpp  ${PROJECT_SOURCE_DIR}/main.cpp )

    link_directories(/home-isis/auto/dapi/s18theve/xp/include /home-isis/auto/dapi/s18theve/xp/\\lib)
    include_directories(/home-isis/auto/dapi/s18theve/xp/include /home-isis/auto/dapi/s18theve/xp/lib)
    target_link_libraries(untitled /home-isis/auto/dapi/s18theve/xp/lib/xprs.lib)
ENDIF(WIN32)



link_libraries(xprs.lib)
add_library(xprs SHARED IMPORTED)
link_libraries(xprb.lib)
add_library(xprb SHARED IMPORTED)
link_libraries(xprm_mc.lib)
add_library(xprm_mc SHARED IMPORTED)
link_libraries(xprm_rt.lib)
add_library(xprm_rt SHARED IMPORTED)



