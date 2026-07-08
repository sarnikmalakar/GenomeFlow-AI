#include <benchmark/benchmark.h>

#include <random>
#include <vector>

#include "population.hpp"
#include "simulation.hpp"
#include "selection.hpp"
#include "mutation.hpp"

// ======================================================
// Fitness Kernel Benchmark
// ======================================================

static void BM_FitnessKernel(benchmark::State& state)
{
    const size_t N = state.range(0);

    Population pop(N);

    for(auto _ : state)
    {
        compute_population_fitness(pop);

        benchmark::ClobberMemory();
    }

    state.SetItemsProcessed(
        state.iterations() * N);
}

BENCHMARK(BM_FitnessKernel)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Arg(1000000);

// ======================================================
// Mutation Kernel Benchmark
// ======================================================

static void BM_MutationKernel(benchmark::State& state)
{
    const size_t N = state.range(0);

    Population pop(N);

    std::mt19937_64 rng(42);

    constexpr double mutation_rate = 0.0001;

    for(auto _ : state)
    {
        mutate_population(
            pop,
            rng,
            mutation_rate);

        benchmark::ClobberMemory();
    }

    state.SetItemsProcessed(
        state.iterations() * N);
}

BENCHMARK(BM_MutationKernel)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Arg(1000000);

// ======================================================
// Selection Kernel Benchmark
// ======================================================

static void BM_SelectionKernel(benchmark::State& state)
{
    const size_t N = state.range(0);

    Population pop(N);
    Population next_pop(N);

    std::vector<double> cumulative(N);

    std::mt19937_64 rng(42);

    compute_population_fitness(pop);

    for(auto _ : state)
    {
        reproduce_population(
            pop,
            next_pop,
            cumulative,
            rng);

        benchmark::ClobberMemory();
    }

    state.SetItemsProcessed(
        state.iterations() * N);
}

BENCHMARK(BM_SelectionKernel)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Arg(1000000);

// ======================================================
// Whole Simulation Benchmark
// ======================================================

static void BM_WholeSimulation(benchmark::State& state)
{
    const size_t N = state.range(0);

    Population pop(N);
    Population next_pop(N);

    std::vector<double> cumulative(N);

    std::mt19937_64 rng(42);

    constexpr double mutation_rate = 0.0001;

    for(auto _ : state)
    {
        compute_population_fitness(pop);

        reproduce_population(
            pop,
            next_pop,
            cumulative,
            rng);

        mutate_population(
            next_pop,
            rng,
            mutation_rate);

        pop.genome.swap(next_pop.genome);
        pop.age.swap(next_pop.age);
        pop.passenger_mutations.swap(
            next_pop.passenger_mutations);

        benchmark::ClobberMemory();
    }

    state.SetItemsProcessed(
        state.iterations() * N);
}

BENCHMARK(BM_WholeSimulation)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Arg(1000000);

// ======================================================

BENCHMARK_MAIN();