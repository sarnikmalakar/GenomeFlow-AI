#pragma once

#include <random>
#include<vector>

#include "population.hpp"

void reproduce_population(
    const Population& pop,
    Population& next_pop,
    std::vector<double>& cumulative,
    std::mt19937_64& rng
);