#include "dataset.hpp"
#include "population.hpp"
#include "genome.hpp"

#include <fstream>
#include <iostream>

void export_dataset(
    const Population& pop,
    const std::string& filename,
    int generation)
{
    std::ofstream file(filename);

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

    file
        << "cell_id,"
        << "generation,"
        << "tp53,"
        << "kras,"
        << "egfr,"
        << "brca1,"
        << "brca2,"
        << "apc,"
        << "pik3ca,"
        << "pten,"
        << "mutation_count,"
        << "fitness\n";

    // =========================
    // Export Every Cell
    // =========================

    for(size_t i = 0; i < pop.genome.size(); i++)
    {
        Genome genome = pop.genome[i];

        bool tp53 =
            (genome & (1u << static_cast<int>(Gene::TP53))) != 0;

        bool kras =
            (genome & (1u << static_cast<int>(Gene::KRAS))) != 0;

        bool egfr =
            (genome & (1u << static_cast<int>(Gene::EGFR))) != 0;

        bool brca1 =
            (genome & (1u << static_cast<int>(Gene::BRCA1))) != 0;

        bool brca2 =
            (genome & (1u << static_cast<int>(Gene::BRCA2))) != 0;

        bool apc =
            (genome & (1u << static_cast<int>(Gene::APC))) != 0;

        bool pik3ca =
            (genome & (1u << static_cast<int>(Gene::PIK3CA))) != 0;

        bool pten =
            (genome & (1u << static_cast<int>(Gene::PTEN))) != 0;

        int mutation_count = 0;

        for(int g = 0;
            g < static_cast<int>(Gene::COUNT);
            g++)
        {
            if(genome & (1u << g))
            {
                mutation_count++;
            }
        }

        file
            << i << ","
            << generation << ","
            << tp53 << ","
            << kras << ","
            << egfr << ","
            << brca1 << ","
            << brca2 << ","
            << apc << ","
            << pik3ca << ","
            << pten << ","
            << mutation_count << ","
            << pop.fitness[i]
            << '\n';
    }

    file.close();

    std::cout
        << "Dataset exported successfully to "
        << filename
        << '\n';
}