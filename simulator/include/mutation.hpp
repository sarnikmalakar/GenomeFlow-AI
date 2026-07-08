#pragma once

#include <random>

#include "population.hpp"

void mutate(
    Genome& genome,
    std::mt19937_64& rng);

void mutate_population(
    Population& pop,
    std::mt19937_64& rng,
    double mutation_rate);