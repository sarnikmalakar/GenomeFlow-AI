cmake_minimum_required(VERSION 3.22)

project(GenomeFlowAI)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenMP REQUIRED)

add_executable(
    genomeflow_cpu
    simulator/cpu/main.cpp
    simulator/cpu/fitness.cpp
    simulator/cpu/clone.cpp
)

target_include_directories(
    genomeflow_cpu
    PRIVATE
    simulator/include
)

target_link_libraries(
    genomeflow_cpu
    PRIVATE
    OpenMP::OpenMP_CXX
)


find_package(benchmark REQUIRED)

add_executable(
    cpu_benchmark
    simulator/benchmark/cpu_benchmark.cpp
    simulator/cpu/fitness.cpp
)

target_include_directories(
    cpu_benchmark
    PRIVATE
    simulator/include
)

target_link_libraries(
    cpu_benchmark
    PRIVATE
    benchmark::benchmark
    benchmark::benchmark_main
)

