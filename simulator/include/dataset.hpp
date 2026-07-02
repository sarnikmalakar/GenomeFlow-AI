#pragma once
#include<string>

#include "clone_stats.hpp"

struct Population;

void export_clone_dataset(
    const std::vector<CloneStats>& clones,
    const std::string& filename,
    bool write_header);