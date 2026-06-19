# GenomeFlow AI Benchmark Report

## System Information

CPU: Intel(R) Core(TM) i3-4150 @ 3.50 GHz
RAM: 5.2 GB
Compiler: g++ 15.2.0
OS: Ubuntu Linux
Build: Serial CPU

## Configuration

Cells: 1,000,000
Generations: 100
Mutation Rate: 0.0001
Total Cell Updates: 100,000,000

## Scientific Results

Final Average Fitness: 3.2526
Mutated Cells: 1,000,000
Number of Clones: 7
Largest Clone Size: 984,154

### Dominant Clones

TP53|KRAS|EGFR: 984,154
TP53|KRAS|EGFR|PIK3CA: 8,763
TP53|KRAS|EGFR|PTEN: 2,379

### Mutation Counts

TP53: 1,000,000
KRAS: 1,000,000
EGFR: 1,000,000
BRCA1: 1,633
BRCA2: 1,492
APC: 1,586
PIK3CA: 8,763
PTEN: 2,379

## Performance Metrics

Runtime: 95.47 sec
User Time: 95.35 sec
System Time: 0.03 sec
CPU Utilization: 1 Core

CPU Cycles: 330,885,691,161
Instructions: 273,400,580,216
IPC: 0.83
CPU Frequency: 3.50 GHz

Branches: 32,171,406,652
Branch Misses: 1,289,384,304
Branch Miss Rate: 4.0%

Context Switches: 1,465
CPU Migrations: 64
Page Faults: 5,107

## CPU Kernel Benchmark Results

| Kernel             | 1,000,000 Elements |
| ------------------ | ------------------ |
| Fitness Evaluation | 22.11 ms           |
| Mutation Engine    | 21.86 ms           |
| Clone Counting     | 57.96 ms           |
| Parent Selection   | 657.67 ms          |

### Bottleneck Analysis

Parent Selection is the dominant computational cost.

Relative Cost:

* Selection: 29.8× Fitness
* Clone Counting: 2.6× Fitness
* Mutation: 1.0× Fitness

Primary Optimization Target:

1. Parallel Parent Selection (OpenMP)
2. Parallel Clone Counting
3. SIMD Optimization
4. HIP/ROCm GPU Port

