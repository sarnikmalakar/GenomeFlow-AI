#pragma once

#include <string>
#include <cstddef>

#include "genome.hpp"
#include "population.hpp"

struct CloneInfo
{
    Genome genome;
    size_t count;
};

std::string genome_to_string(Genome genome);