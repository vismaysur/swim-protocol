cmake_minimum_required(VERSION 3.28.3)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(SWIM)

add_library(
  networking_source
  src/networking.cpp
)

target_include_directories(
  networking_source PRIVATE
  include/
)

add_executable(
  ${PROJECT_NAME}
  src/main.cpp
)

target_link_libraries(
  ${PROJECT_NAME}
  networking_source
)

target_include_directories(
  ${PROJECT_NAME} PRIVATE
  include/
)

add_library(
  fsa_source 
  src/fsa.cpp
)

target_include_directories(
  fsa_source PRIVATE
  include/
)

# TESTING

include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  fsa_test
  test/FiniteStateMachineSpec.cpp
)

target_link_libraries(
  fsa_test
  GTest::gtest_main
  fsa_source
)

target_include_directories(
  fsa_test PRIVATE
  include/
)

include(GoogleTest)
gtest_discover_tests(fsa_test)
