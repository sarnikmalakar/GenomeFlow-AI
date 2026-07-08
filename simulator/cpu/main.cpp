#include <iostream>
#include <random>
#include <vector>
#include <unordered_map>
#include <algorithm>

#include "population.hpp"
#include "fitness.hpp"
#include "clone.hpp"
#include "dataset.hpp"
#include "clone_stats.hpp"
#include "simulation.hpp"
#include "selection.hpp"
#include "mutation.hpp"


void run_simulation(int simulation_id,uint64_t seed){
    constexpr size_t NUM_CELLS = 1000000;
    constexpr int GENERATIONS = 100;
    constexpr double mutation_rate = 0.0001;

    Population pop(NUM_CELLS);
    std::mt19937_64 rng(seed);

    Population next_pop(NUM_CELLS);
    std::vector<double>
        cumulative(NUM_CELLS);

    // =========================
    // Evolution Simulation
    // =========================

    for(int gen = 0; gen < GENERATIONS; gen++)
    {
        // Fitness
        compute_population_fitness(pop);

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

        reproduce_population(
        pop,
        next_pop,
        cumulative,
        rng
    );

        // Mutation

        mutate_population(
        next_pop,
        rng,
        mutation_rate);

        // Replace Generation

        pop.genome.swap(next_pop.genome);

        pop.age.swap(next_pop.age);

        pop.passenger_mutations.swap(next_pop.passenger_mutations);

        compute_population_fitness(pop);

        auto clone_stats =
        compute_clone_stats(
            pop,simulation_id,
            gen);

        export_clone_dataset(
            clone_stats,
            "clone_dataset.csv",
            simulation_id == 0 && gen == 0);


    }

    
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
}


int main()
{
    constexpr int NUM_SIMULATIONS = 10;

    for(int sim = 0;
        sim < NUM_SIMULATIONS;
        sim++)
    {
        run_simulation(
            sim,
            42 + sim);
    }


    return 0;
}