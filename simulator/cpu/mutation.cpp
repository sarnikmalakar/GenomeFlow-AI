#include "mutation.hpp"

#include <algorithm>
#include <random>

#include "fitness.hpp"

void mutate(
    Genome& genome,
    std::mt19937_64& rng)
{
    static std::discrete_distribution<int> gene_dist(
    {
        0.30,   // TP53
        0.22,   // KRAS
        0.15,   // EGFR
        0.05,   // BRCA1
        0.05,   // BRCA2
        0.08,   // APC
        0.10,   // PIK3CA
        0.05    // PTEN
    });

    int gene = gene_dist(rng);

    genome |= (1u << gene);
}

void mutate_population(
    Population& pop,
    std::mt19937_64& rng,
    double mutation_rate)
{
    std::uniform_real_distribution<double>
        prob(0.0, 1.0);

    const size_t num_cells = pop.genome.size();

    for(size_t i = 0; i < num_cells; i++)
    {
        CellState state =
            calculate_cell_state(
                pop.genome[i],
                pop.age[i],
                pop.passenger_mutations[i]);

        double actual_mutation_rate =
            mutation_rate *
            state.mutation_rate *
            (2.0 - state.dna_repair);

        actual_mutation_rate =
            std::clamp(
                actual_mutation_rate,
                0.0,
                1.0);

        if(prob(rng) < actual_mutation_rate)
        {
            pop.passenger_mutations[i]++;

            if(prob(rng) < 0.05)
            {
                mutate(
                    pop.genome[i],
                    rng);
            }
        }
    }
}