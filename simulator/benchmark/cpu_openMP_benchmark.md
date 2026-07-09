# GenomeFlowAI CPU + OpenMP Benchmark Report

## Overview

This benchmark evaluates the performance impact of OpenMP acceleration on the core computational kernels of GenomeFlowAI.

The simulator models the evolution of **1,000,000 virtual cells** over multiple generations by repeatedly executing:

- Fitness Evaluation
- Mutation Simulation
- Roulette Wheel Selection
- Complete Population Simulation

OpenMP was introduced to parallelize the most computationally intensive kernels while preserving deterministic biological behavior.

---

# Benchmark Environment

| Component | Specification |
|-----------|---------------|
| CPU | 4-Core @ 3.50 GHz |
| Compiler | GCC (C++20) |
| Parallel Framework | OpenMP |
| Benchmark Library | Google Benchmark |
| Population Size | 1,000,000 Cells |
| Generations | 100 |

---

# Optimization Summary

The following kernels were parallelized using OpenMP:

- ✅ Fitness Evaluation
- ✅ Mutation Kernel
- ✅ Selection (Parent Reproduction)
- ✅ Population Statistics

The implementation uses:

- Static scheduling
- Thread-local random number generators
- Embarrassingly parallel kernels
- Cache-friendly Structure-of-Arrays (SoA) memory layout

---

# Benchmark Results

| Kernel | Before OpenMP | After OpenMP | Speedup |
|---------|--------------:|-------------:|---------:|
| Fitness Kernel | **86.18 ms** | **38.10 ms** | **2.26×** |
| Mutation Kernel | **226.77 ms** | **82.74 ms** | **2.74×** |
| Selection Kernel | **942.79 ms** | **311.31 ms** | **3.03×** |
| Whole Simulation | **1277.92 ms** | **450.56 ms** | **2.84×** |

---

# Throughput Comparison

## Fitness Kernel

| Version | Throughput |
|----------|-----------:|
| Sequential | 11.60 Million Cells/sec |
| OpenMP | 26.25 Million Cells/sec |

**2.26× improvement**

---

## Mutation Kernel

| Version | Throughput |
|----------|-----------:|
| Sequential | 4.41 Million Cells/sec |
| OpenMP | 12.09 Million Cells/sec |

**2.74× improvement**

---

## Selection Kernel

| Version | Throughput |
|----------|-----------:|
| Sequential | 1.06 Million Cells/sec |
| OpenMP | 3.21 Million Cells/sec |

**3.03× improvement**

---

## Whole Simulation

| Version | Throughput |
|----------|-----------:|
| Sequential | 0.78 Million Cells/sec |
| OpenMP | 2.22 Million Cells/sec |

**2.84× improvement**

---

# Performance Analysis

## Fitness Evaluation

Each cell is processed independently.

Parallelization strategy:

- OpenMP `parallel for`
- Static scheduling
- No synchronization required

Result:

- **2.26× speedup**

---

## Mutation Kernel

Each thread operates on an independent subset of cells.

Optimization:

- Thread-local random number generators
- Independent mutation pipeline
- Static scheduling

Result:

- **2.74× speedup**

---

## Selection Kernel

Roulette-wheel selection consists of two stages:

1. Prefix Sum (Sequential)
2. Parent Selection (Parallel)

Only the parent-selection stage was parallelized.

Although the cumulative prefix sum remains sequential, the kernel still achieved:

- **3.03× speedup**

making it the largest optimization in the simulator.

---

## Whole Population Simulation

The complete simulation pipeline includes:

1. Fitness Evaluation
2. Parent Selection
3. Mutation
4. Clone Statistics
5. Dataset Generation

Overall execution time decreased from

**1.28 seconds**

to

**0.45 seconds**

representing an overall **2.84× acceleration**.

---

# Scalability

The simulator now efficiently parallelizes computation across available CPU cores while maintaining deterministic biological behavior.

Key design decisions include:

- Structure-of-Arrays (SoA) memory layout
- Thread-local random number generators
- Independent per-cell computation
- OpenMP static scheduling
- Lock-free fitness and mutation kernels

---

# Future Optimizations

Potential future improvements include:

- Parallel prefix-sum implementation
- Alias-method based roulette-wheel selection
- SIMD vectorization of fitness computation
- NUMA-aware memory allocation
- HIP/ROCm GPU acceleration
- Lock-free clone counting
- Cache-aware clone analysis

---

# Conclusion

OpenMP acceleration significantly improved the performance of GenomeFlowAI's CPU simulator.

The optimized implementation achieved:

- **2.26× faster Fitness Evaluation**
- **2.74× faster Mutation Simulation**
- **3.03× faster Parent Selection**
- **2.84× faster End-to-End Simulation**

These optimizations enable efficient simulation of **one million virtual cells** while maintaining biologically inspired mutation, selection, and clonal evolution dynamics.