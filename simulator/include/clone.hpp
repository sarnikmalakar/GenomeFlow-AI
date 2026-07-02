#pragma once

#include <string>
#include <cstddef>

#include "genome.hpp"

struct CloneInfo
{
    Genome genome;
    size_t count;
};

std::string genome_to_string(Genome genome);