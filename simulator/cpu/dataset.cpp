#include "dataset.hpp"
#include "clone_stats.hpp"
#include "clone.hpp"

#include <fstream>
#include <iostream>

void export_clone_dataset(
    const std::vector<CloneStats>& clones,
    const std::string& filename,
    bool write_header)
{
    std::ofstream file;

    if(write_header)
    {
        file.open(filename);
    }
    else
    {
        file.open(filename, std::ios::app);
    }

    if(!file.is_open())
    {
        std::cerr << "Failed to open file: "
                  << filename
                  << '\n';
        return;
    }

    // =========================
    // CSV Header
    // =========================
    if(write_header)
    {
        file
            << "simulation_id,"
            << "generation,"
            << "clone,"
            << "clone_size,"
            << "clone_fraction,"
            << "average_fitness,"
            << "average_age,"
            << "average_passenger_mutations,"
            << "driver_mutation_count,"
            << "aggressiveness_score\n";
            //<< "aggressive\n";
    }
    // =========================
    // Export Every Cell
    // =========================

    for(const CloneStats& clone : clones)
{
    file
    << clone.simulation_id << ","
    << clone.generation << ","
    << genome_to_string(clone.genome) << ","
    << clone.clone_size << ","
    << clone.clone_fraction << ","
    << clone.average_fitness << ","
    << clone.average_age << ","
    << clone.average_passenger_mutations << ","
    << static_cast<int>(clone.driver_mutation_count) << ","
    << clone.aggressiveness_score 
    //<< clone.aggressive
    << '\n';
}

    file.close();

    std::cout
        << "Dataset exported successfully to "
        << filename
        << '\n';
}