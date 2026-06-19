#include <benchmark/benchmark.h>

#include <algorithm>
#include <random>
#include <unordered_map>
#include <vector>

#include "fitness.hpp"
#include "genome.hpp"

// ======================================================
// Fitness Kernel Benchmark
// ======================================================

static void BM_FitnessKernel(
    benchmark::State& state)
{
    const size_t N = state.range(0);

    std::vector<Genome> genomes(
        N,
        0b111);

    std::vector<float> fitness(
        N);

    for(auto _ : state)
    {
        for(size_t i = 0; i < N; i++)
        {
            fitness[i] =
                calculate_fitness(
                    genomes[i]);
        }

        benchmark::ClobberMemory();
    }
}

BENCHMARK(BM_FitnessKernel)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Arg(1000000);

// ======================================================
// Mutation Kernel Benchmark
// ======================================================

static void BM_MutationKernel(
    benchmark::State& state)
{
    const size_t N = state.range(0);

    std::vector<Genome> genomes(
        N,
        0);

    std::mt19937_64 rng(42);

    for(auto _ : state)
    {
        for(size_t i = 0; i < N; i++)
        {
            int gene = rng() % 8;

            genomes[i] |=
                (1u << gene);
        }

        benchmark::ClobberMemory();
    }
}

BENCHMARK(BM_MutationKernel)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Arg(1000000);

// ======================================================
// Selection Kernel Benchmark
// ======================================================

static void BM_SelectionKernel(
    benchmark::State& state)
{
    const size_t N = state.range(0);

    std::vector<double> cumulative(
        N);

    for(size_t i = 0; i < N; i++)
    {
        cumulative[i] =
            static_cast<double>(i + 1);
    }

    std::mt19937_64 rng(42);

    std::uniform_real_distribution<double>
        dist(
            0.0,
            cumulative.back());

    for(auto _ : state)
    {
        for(size_t i = 0; i < N; i++)
        {
            double r =
                dist(rng);

            auto it =
                std::lower_bound(
                    cumulative.begin(),
                    cumulative.end(),
                    r);

            benchmark::DoNotOptimize(
                it);
        }
    }
}

BENCHMARK(BM_SelectionKernel)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Arg(1000000);

// ======================================================
// Clone Counting Benchmark
// ======================================================

static void BM_CloneCounting(
    benchmark::State& state)
{
    const size_t N = state.range(0);

    std::vector<Genome> genomes(
        N);

    for(size_t i = 0; i < N; i++)
    {
        genomes[i] =
            static_cast<Genome>(
                i % 256);
    }

    for(auto _ : state)
    {
        std::unordered_map<
            Genome,
            size_t> clone_counts;

        for(auto genome : genomes)
        {
            clone_counts[genome]++;
        }

        benchmark::ClobberMemory();
    }
}

BENCHMARK(BM_CloneCounting)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Arg(1000000);

// ======================================================
// Benchmark Entry Point
// ======================================================

BENCHMARK_MAIN();

