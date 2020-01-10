cmake_minimum_required (VERSION 3.3)
project (triangular_solve)

set(CMAKE_CXX_FLAGS "-O0 --std=gnu++17 -fopenmp")
set(CMAKE_C_FLAGS "-O0 -fopenmp")
set(CMAKE_EXE_LINKER_FLAGS "-lgomp")

add_executable(triangular_solve
  source/main.cpp

)

target_include_directories(triangular_solve PUBLIC
  include
)
