cmake_minimum_required(VERSION 3.14)
project(ransac_sigma_test)

set(CMAKE_CXX_STANDARD 14)


find_package(Eigen3 3.1.0 REQUIRED)
include_directories(
        ${EIGEN3_INCLUDE_DIR})

add_executable(ransac_sigma_test main.cpp Methods.cpp Methods.h RANSAC.cpp RANSAC.h SP.cpp SP.h)
target_link_libraries(ransac_sigma_test ${EIGEN3_LIBS})