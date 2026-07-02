#pragma once

#include <vector>
#include <cstddef>
#include <cstdint>

#include "genome.hpp"

struct Population
{
    std::vector<Genome> genome;
    std::vector<float> fitness;

    // Number of cell divisions
    std::vector<uint16_t> age;

    // Number of passenger mutations
    std::vector<uint16_t> passenger_mutations;

    Population(size_t n)
        : genome(n, 0),
          fitness(n, 1.0f),
          age(n, 0),
          passenger_mutations(n, 0)
    {}
};