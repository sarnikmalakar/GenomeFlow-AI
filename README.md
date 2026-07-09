# GenomeFlowAI

> A high-performance cancer evolution simulator and explainable machine learning pipeline built in C++20 and Python.

GenomeFlowAI simulates the evolutionary dynamics of cancer cell populations using biologically inspired mutation models and generates large-scale synthetic genomic datasets for machine learning. The project combines high-performance systems programming with explainable AI to study tumor evolution and predict clone aggressiveness.

---

# Features

- High-performance cancer evolution simulator written in **C++20**
- Biologically inspired mutation model for major driver genes
- Simulation of **1,000,000+ virtual cells**
- Multi-generation clonal evolution
- Passenger and driver mutation modelling
- Clone statistics and dataset generation
- Automated preprocessing pipeline
- XGBoost-based aggressiveness prediction
- SHAP explainability
- Google Benchmark performance profiling

---

# Biology Model

GenomeFlowAI models mutations in several clinically important driver genes:

| Gene | Biological Effect |
|-------|-------------------|
| TP53 | DNA repair reduction, apoptosis suppression |
| KRAS | Increased proliferation |
| EGFR | Increased cell division |
| BRCA1 | DNA repair deficiency |
| BRCA2 | DNA repair deficiency |
| APC | Increased proliferation |
| PIK3CA | Growth signalling activation |
| PTEN | Reduced apoptosis |

Each virtual cell maintains:

- Driver mutations
- Passenger mutation count
- Age
- Fitness
- Division probability
- Death probability
- DNA repair efficiency

The simulator models

- Mutation
- Cell ageing
- Clonal evolution
- Natural selection
- Population dynamics

---

# Machine Learning Pipeline

The generated clone dataset is processed through an end-to-end ML pipeline.

```
Simulation
        │
        ▼
Clone Dataset
        │
        ▼
Preprocessing
        │
        ▼
Feature Engineering
        │
        ▼
XGBoost Training
        │
        ▼
Evaluation
        │
        ▼
SHAP Explainability
        │
        ▼
Prediction
```

Features used for training:

- Generation
- Clone Size
- Clone Fraction
- Average Fitness
- Average Age
- Passenger Mutation Count
- Driver Mutation Count

---

# Project Architecture

```
GenomeFlowAI
│
├── simulator
│   ├── cpu
│   │   ├── main.cpp
│   │   ├── simulation.cpp
│   │   ├── fitness.cpp
│   │   ├── mutation.cpp
│   │   ├── selection.cpp
│   │   ├── clone.cpp
│   │   ├── clone_stats.cpp
│   │   └── dataset.cpp
│   │
│   ├── benchmark
│   │   └── cpu_benchmark.cpp
│   │
│   └── include
│
├── ml
│   ├── preprocess.py
│   ├── train.py
│   ├── predict.py
│   └── reports
│
└── CMakeLists.txt
```

---

# Performance Benchmark

Benchmarked using **Google Benchmark**.

## Fitness Kernel

| Cells | Runtime |
|---------|---------|
| 1K | 85 μs |
| 10K | 843 μs |
| 100K | 8.50 ms |
| 1M | 86.18 ms |

Throughput

**11.6 Million Cells / second**

---

## Mutation Kernel

Throughput

**4.4 Million Cells / second**

---

## Selection Kernel

Throughput

**1.06 Million Cells / second**

Selection was identified as the primary computational bottleneck due to cumulative fitness search and binary search based parent selection.

---

# Machine Learning Results

Model

- XGBoost Classifier

Metrics

| Metric | Score |
|---------|-------|
| Accuracy | 99.91% |
| Precision | 99.64% |
| Recall | 100% |
| F1 Score | 99.82% |
| ROC-AUC | 1.00 |

Explainability

- SHAP Summary Plot
- Feature Importance
- ROC Curve
- Confusion Matrix

---

# Build

Clone

```bash
git clone https://github.com/<your-username>/GenomeFlowAI.git
```

Configure

```bash
cmake -B build
```

Build

```bash
cmake --build build
```

Run simulator

```bash
./build/genomeflow_cpu
```

Run benchmark

```bash
./build/cpu_benchmark
```

---

# ML Pipeline

Install dependencies

```bash
cd ml

uv sync
```

Preprocess

```bash
uv run preprocess.py
```

Train

```bash
uv run train.py
```

Predict

```bash
uv run predict.py
```

---

# Generated Reports

Training automatically generates

- Feature Importance
- SHAP Summary
- ROC Curve
- Confusion Matrix

under

```
ml/reports/
```

---

# Future Work

- SIMD vectorization (AVX2 / AVX-512)
- NUMA-aware memory allocation
- Multi-socket scalability
- Additional cancer signalling pathways
- Transformer-based genomic models
- Distributed simulation
- Real genomic dataset validation

---

# Technologies

### Systems

- C++20
- OpenMP
- Google Benchmark
- CMake

### Machine Learning

- Python
- XGBoost
- SHAP
- Pandas
- NumPy
- Scikit-learn

---

# License

MIT License

---

# Author

**Sarnik Malakar**

Computer Science Engineer

Interested in High Performance Computing, Systems Programming and Machine Learning Infrastructure.