# GenomeFlow AI Benchmark Report

## System Information

CPU: Intel(R) Core(TM) i3-4150 @ 3.50 GHz
RAM: 5.2 GB
Compiler: g++ 15.2.0
OS: Ubuntu Linux
Build: Serial CPU

# GenomeFlowAI CPU Benchmark Report

**Date:** 2026-07-08

## Overview

This report summarizes the CPU benchmark results for the GenomeFlowAI
simulator. The benchmarks were performed using Google Benchmark on the
CPU implementation prior to GPU (HIP/ROCm) acceleration.

## Test Environment

-   CPU: 4 × 3.50 GHz
-   L1 Data Cache: 32 KiB ×2
-   L1 Instruction Cache: 32 KiB ×2
-   L2 Cache: 256 KiB ×2
-   L3 Cache: 3072 KiB
-   Benchmark Framework: Google Benchmark

> Note: CPU frequency scaling was enabled during benchmarking, so minor
> runtime variation between runs is expected.

## Benchmark Results

### Fitness Kernel

        Cells    Time (ns)        Throughput
  ----------- ------------ -----------------
        1,000       85,204   11.74 M cells/s
       10,000      843,321   11.86 M cells/s
      100,000    8,496,352   11.77 M cells/s
    1,000,000   86,176,463   11.60 M cells/s

**Observation**

-   Nearly linear scaling.
-   Stable throughput around **11.6--11.9 million cells/second**.
-   Excellent candidate for GPU acceleration.

------------------------------------------------------------------------

### Mutation Kernel

        Cells     Time (ns)       Throughput
  ----------- ------------- ----------------
        1,000       220,468   4.54 M cells/s
       10,000     2,198,251   4.55 M cells/s
      100,000    22,719,592   4.40 M cells/s
    1,000,000   226,773,836   4.41 M cells/s

**Observation**

-   Linear scaling.
-   Random number generation and branching reduce throughput.
-   Strong GPU candidate.

------------------------------------------------------------------------

### Selection Kernel

        Cells     Time (ns)       Throughput
  ----------- ------------- ----------------
        1,000       401,721   2.49 M cells/s
       10,000     4,637,841   2.16 M cells/s
      100,000    59,545,440   1.68 M cells/s
    1,000,000   942,792,573   1.06 M cells/s

**Observation**

-   Slowest stage of the simulator.
-   Dominated by cumulative fitness search (`lower_bound`).
-   Primary CPU bottleneck.

------------------------------------------------------------------------

### Whole Simulation

        Cells       Time (ns)        Throughput
  ----------- --------------- -----------------
        1,000         717,797    1.39 M cells/s
       10,000       7,721,473    1.30 M cells/s
      100,000      98,223,210    1.02 M cells/s
    1,000,000   1,277,919,195   782.5 K cells/s

**Observation**

-   End-to-end runtime scales approximately linearly with population
    size.
-   Selection dominates total execution time.

## Bottleneck Analysis

Estimated contribution for the 1,000,000-cell benchmark:

  Component     Approximate Share
  ----------- -------------------
  Selection                 \~74%
  Mutation                  \~18%
  Fitness                    \~7%
  Other                      \~1%

## Conclusions

-   The simulator exhibits good linear scaling across tested population
    sizes.
-   Fitness and mutation kernels are highly parallel and suitable for
    GPU acceleration.
-   Selection is the primary computational bottleneck and may require
    algorithmic optimization in addition to GPU acceleration.
-   These measurements establish the CPU performance baseline for future
    HIP/ROCm comparisons.

## Next Steps

1.  Implement HIP fitness kernel.
2.  Validate GPU results against CPU output.
3.  Benchmark CPU vs GPU fitness kernel.
4.  Port mutation kernel to HIP.
5.  Investigate optimized GPU strategy for selection.