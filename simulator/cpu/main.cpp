#include <iostream>
#include <random>
#include <vector>
#include <unordered_map>
#include <algorithm>

#include "population.hpp"
#include "fitness.hpp"
#include "clone.hpp"
#include "dataset.hpp"

void mutate(
    Genome& genome,
    std::mt19937_64& rng)
{
    static std::discrete_distribution<int> gene_dist(
{
    0.30,   // TP53
    0.22,   // KRAS
    0.15,   // EGFR
    0.05,   // BRCA1
    0.05,   // BRCA2
    0.08,   // APC
    0.10,   // PIK3CA
    0.05    // PTEN
});
    int gene = gene_dist(rng);
    genome |= (1u << gene);
}

int main()
{
    constexpr size_t NUM_CELLS = 1000000;
    constexpr int GENERATIONS = 100;
    constexpr double mutation_rate = 0.0001;

    Population pop(NUM_CELLS);
    std::mt19937_64 rng(42);
    std::uniform_real_distribution<double>
        prob(0.0, 1.0);
    Population next_pop(NUM_CELLS);
    std::vector<double>
        cumulative(NUM_CELLS);

    // =========================
    // Evolution Simulation
    // =========================

    for(int gen = 0; gen < GENERATIONS; gen++)
    {
        // Fitness
        for(size_t i = 0; i < NUM_CELLS; i++)
        {
            CellState state = calculate_cell_state(pop.genome[i],pop.age[i],pop.passenger_mutations[i]);

            pop.fitness[i] = calculate_fitness(state);
        }

        // Progress Logging
        if(gen % 10 == 0)
        {
            double avg_fitness = 0.0;

            for(float f : pop.fitness)
            {
                avg_fitness += f;
            }

            avg_fitness /= NUM_CELLS;

            std::cout
                << "Generation "
                << gen
                << " Average Fitness: "
                << avg_fitness
                << '\n';
        }

        // Cumulative Fitness

        cumulative[0] =
            pop.fitness[0];

        for(size_t i = 1; i < NUM_CELLS; i++)
        {
            cumulative[i] =
                cumulative[i - 1]
                + pop.fitness[i];
        }

        double total_fitness =
            cumulative.back();

        std::uniform_real_distribution<double>
            parent_dist(
                0.0,
                total_fitness);

        // Selection + Reproduction

        for(size_t i = 0; i < NUM_CELLS; i++)
        {
            double r =
                parent_dist(rng);

            auto it =
                std::lower_bound(
                    cumulative.begin(),
                    cumulative.end(),
                    r);

            size_t parent = std::min(static_cast<size_t>(std::distance(cumulative.begin(),it)),NUM_CELLS-1);

            next_pop.genome[i] =
                pop.genome[parent];
            next_pop.age[i] =
                pop.age[parent]+1;
            next_pop.passenger_mutations[i] =
                pop.passenger_mutations[parent];
        }

        // Mutation

        for(size_t i = 0; i < NUM_CELLS; i++)
{
    CellState state =
        calculate_cell_state(next_pop.genome[i],next_pop.age[i],next_pop.passenger_mutations[i]);

    double actual_mutation_rate =
        mutation_rate *
        state.mutation_rate *
        (2.0 - state.dna_repair);

    actual_mutation_rate = std::clamp(actual_mutation_rate, 0.0, 1.0);

    if(prob(rng) < actual_mutation_rate)
    {
        next_pop.passenger_mutations[i]++;

        if(prob(rng) < 0.05)
        {
            mutate(next_pop.genome[i], rng);
        }
    }
}

        // Replace Generation

        pop.genome.swap(next_pop.genome);

        pop.age.swap(next_pop.age);

        pop.passenger_mutations.swap(next_pop.passenger_mutations);
    }

    // =========================
    // Final Fitness
    // =========================

    for(size_t i = 0; i < NUM_CELLS; i++)
    {
        CellState state = calculate_cell_state(pop.genome[i],pop.age[i],pop.passenger_mutations[i]);

        pop.fitness[i] = calculate_fitness(state);
    }
    // Export synthetic dataset
    export_dataset(pop, "cells.csv", GENERATIONS);

    
    // =========================
    // Clone Analysis
    // =========================

    std::unordered_map<Genome, size_t>
        clone_counts;

    std::vector<size_t> gene_counts(
        static_cast<int>(
            Gene::COUNT),
        0);

    for(auto genome : pop.genome)
    {
        clone_counts[genome]++;

        for(int g = 0;
            g < static_cast<int>(
                    Gene::COUNT);
            g++)
        {
            if(genome & (1u << g))
            {
                gene_counts[g]++;
            }
        }
    }

    // Largest Clone

    size_t largest_clone = 0;

    for(const auto& [genome, count]
        : clone_counts)
    {
        largest_clone =
            std::max(
                largest_clone,
                count);
    }

    // Top Clones

    std::vector<CloneInfo>
        clones;

    for(const auto& [genome, count]
        : clone_counts)
    {
        clones.push_back(
            {genome, count});
    }

    std::sort(
        clones.begin(),
        clones.end(),
        [](const CloneInfo& a,
           const CloneInfo& b)
        {
            return a.count >
                   b.count;
        });

    // =========================
    // Statistics
    // =========================

    size_t mutated_cells = 0;

    for(auto genome : pop.genome)
    {
        if(genome != 0)
        {
            mutated_cells++;
        }
    }

    double avg_fitness = 0.0;
    double avg_age = 0.0;
    double avg_passengers = 0.0;
    for(size_t i = 0; i < NUM_CELLS; i++)
    {
        avg_age += pop.age[i];
        avg_passengers += pop.passenger_mutations[i];
    }

    avg_age /= NUM_CELLS;
    avg_passengers /= NUM_CELLS;


    for(float fitness : pop.fitness)
    {
        avg_fitness += fitness;
    }

    avg_fitness /=
        pop.fitness.size();

    // =========================
    // Output
    // =========================

    std::cout
        << "\nTop Clones\n";

    size_t top_n =
        std::min<size_t>(
            10,
            clones.size());

    for(size_t i = 0;
        i < top_n;
        i++)
    {
        std::cout
            << genome_to_string(
                   clones[i].genome)
            << " Count: "
            << clones[i].count
            << '\n';
    }

    std::cout
        << "\nMutated Cells: "
        << mutated_cells
        << '\n';

    std::cout
        << "Average Fitness: "
        << avg_fitness
        << '\n';

    std::cout
        << "Average Age: "
        << avg_age
        << '\n';

    std::cout
        << "Average Passenger Mutations: "
        << avg_passengers
        << '\n';

    std::cout
        << "Number of Clones: "
        << clone_counts.size()
        << '\n';

    std::cout
        << "Largest Clone Size: "
        << largest_clone
        << '\n';

    const char* gene_names[] =
    {
        "TP53",
        "KRAS",
        "EGFR",
        "BRCA1",
        "BRCA2",
        "APC",
        "PIK3CA",
        "PTEN"
    };

    std::cout
        << "\nMutation Counts\n";

    for(int i = 0; i < static_cast<int>(Gene::COUNT); i++)
    {
        std::cout
            << gene_names[i]
            << ": "
            << gene_counts[i]
            << '\n';
    }

    return 0;
}