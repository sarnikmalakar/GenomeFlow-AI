#include "clone_stats.hpp"

#include <unordered_map>
#include <vector>
#include <bit>


#include "population.hpp"

namespace
{
    struct CloneAccumulator
    {
        size_t clone_size = 0;

        double fitness_sum = 0.0;
        double age_sum = 0.0;
        double passenger_sum = 0.0;
    };
}

std::vector<CloneStats>
compute_clone_stats(
    const Population& pop,int simulation_id,
    int generation)
{
    std::unordered_map<Genome, CloneAccumulator> clone_map;

    // ----------------------------------------
    // Accumulate statistics for each clone
    // ----------------------------------------

    for(size_t i = 0; i < pop.genome.size(); i++)
    {

        Genome genome = pop.genome[i];

        auto& clone = clone_map[genome];

        clone.clone_size++;

        clone.fitness_sum += pop.fitness[i];

        clone.age_sum += pop.age[i];

        clone.passenger_sum +=
            pop.passenger_mutations[i];
    }

    // ----------------------------------------
    // Convert to CloneStats
    // ----------------------------------------

    std::vector<CloneStats> stats;

    stats.reserve(clone_map.size());

    for(const auto& [genome, clone] : clone_map)
    {
        CloneStats entry;

        entry.simulation_id = simulation_id;

        entry.generation = generation;

        entry.genome = genome;

        entry.clone_size = clone.clone_size;

        entry.average_fitness =
            static_cast<float>(
                clone.fitness_sum /
                clone.clone_size);

        entry.average_age =
            static_cast<float>(
                clone.age_sum /
                clone.clone_size);

        entry.average_passenger_mutations =
            static_cast<float>(
                clone.passenger_sum /
                clone.clone_size);

        entry.driver_mutation_count =
            static_cast<uint8_t>(
                std::popcount(
                    static_cast<unsigned int>(genome)));

        entry.clone_fraction =
            static_cast<float>(entry.clone_size) /
            static_cast<float>(pop.genome.size());

        entry.aggressiveness_score =
            entry.average_fitness * 0.45f
            + static_cast<float>(entry.driver_mutation_count) * 0.25f
            + entry.clone_fraction * 0.20f
            + entry.average_passenger_mutations * 0.10f;

        //entry.aggressive =
            //entry.aggressiveness_score > 5.0f;

        stats.push_back(entry);
    }

    return stats;
}