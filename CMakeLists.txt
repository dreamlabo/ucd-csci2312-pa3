cmake_minimum_required(VERSION 3.3)
project(PA3_Spring_2016)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Point.cpp Point.h KMeans.cpp KMeans.h  ErrorContext.cpp ErrorContext.h  Cluster.cpp Cluster.h Exceptions.cpp Exceptions.h ClusteringTests.cpp ClusteringTests.h)
add_executable(PA3_Spring_2016 ${SOURCE_FILES})