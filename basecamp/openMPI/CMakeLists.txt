cmake_minimum_required(VERSION 3.10)
project(MyMPIProject)
    
find_package(MPI REQUIRED)
    
add_executable(my_mpi_program count.cpp)
target_link_libraries(my_mpi_program MPI::MPI_CXX)
