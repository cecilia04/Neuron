cmake_minimum_required(VERSION 2.6)
project(neuron)

set(CMAKE_CXX_FLAGS "-O3 -W -Wall -pedantic -std=c++11")

enable_testing()
add_subdirectory(googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(neuron 
			   neuron.cpp 
			   main.cpp
		       cortex.cpp
			   )
add_executable(neuron_test 
			   Neuron_Test.cpp 
			   neuron.cpp
			   cortex.cpp
			   )
				
target_link_libraries(neuron_test
					  gtest
					  gtest_main
					  )
add_test(neuron_test neuron_test)
						
