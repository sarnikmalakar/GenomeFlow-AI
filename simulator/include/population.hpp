#pragma once

#include<vector>
#include <cstddef>
#include "genome.hpp"

struct Population
{
    std::vector<Genome> genome;
    std::vector<float> fitness;

    Population(size_t n)
        : genome(n, 0),
          fitness(n, 1.0f)
    {}
};