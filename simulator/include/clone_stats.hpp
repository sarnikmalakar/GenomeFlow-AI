#pragma once

#include <cstddef>
#include <cstdint>

#include "genome.hpp"
#include "population.hpp"

struct CloneStats
{
    int simulation_id;
    int generation;

    // Clone identity
    Genome genome;

    // Population statistics
    size_t clone_size;
    float clone_fraction;

    // Biological summary
    float average_fitness;
    float average_age;
    float average_passenger_mutations;

    // Genome summary
    uint8_t driver_mutation_count;

    // Evolution
   float aggressiveness_score;

    // ML label
    //bool aggressive;
};

std::vector<CloneStats>
compute_clone_stats(
    const Population& pop,int simulation_id,
    int generation);