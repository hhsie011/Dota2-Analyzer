cmake_minimum_required(VERSION 2.8)

#ADD_SUBDIRECTORY(googletest)

project(backend)

set(CMAKE_CXX_STANDARD 11)
#set(CURL_LIBRARY "-lcurl") 

#find_package(CURL REQUIRED) 

#include_directories(${CURL_INCLUDE_DIR})

#add_executable(test unit_tests/test.cpp)
add_executable(backend.out
        manager.cpp
        manager_functions.cpp
        submanager_ability.cpp
	submanager_ability_upgrades.cpp
        submanager_heroNames.cpp
        submanager_objectives.cpp
        submanager_players.cpp
	submanager_playerRatings.cpp
	submanager_match.cpp
	submanager_cluster.cpp
        )

target_link_libraries(backend.out)
#add_library(curl /usr/include/x86_64-linux-gnu)

#TARGET_LINK_LIBRARIES(backend.out curl)
#TARGET_COMPILE_DEFINITIONS(test PRIVATE gtest_disable_pthreads=ON)
