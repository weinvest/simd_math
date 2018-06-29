cmake_minimum_required(VERSION 2.8)
project(simd_math)
find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++14 -march=native -mavx2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++14 -march=native -mavx2")

#set(SOURCE_FILES DoubleTest.cpp FloatTest.cpp)
#set(SOURCE_FILES DoubleTest.cpp)
set(SOURCE_FILES Int4Test.cpp)

macro(add_test SIMD_Name)
    add_executable(test_${SIMD_Name}
            ${SOURCE_FILES})
    set_target_properties(test_${SIMD_Name} PROPERTIES
            COMPILE_FLAGS "-m${SIMD_Name}")
    target_link_libraries(test_${SIMD_Name}
            ${GTEST_BOTH_LIBRARIES})
    target_link_libraries(test_${SIMD_Name} pthread)
endmacro()

add_test(avx2)
