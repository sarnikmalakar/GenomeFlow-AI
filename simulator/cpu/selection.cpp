#include "selection.hpp"

#include <algorithm>
#include <vector>
#include <random>

void reproduce_population(
    const Population& pop,
    Population& next_pop,
    std::vector<double>& cumulative,
    std::mt19937_64& rng)
{
    const size_t num_cells = pop.genome.size();

    if(cumulative.size() != num_cells)
    {
        cumulative.resize(num_cells);
    }

    cumulative[0] = pop.fitness[0];

    for(size_t i = 1; i < num_cells; i++)
    {
        cumulative[i] =
            cumulative[i - 1] +
            pop.fitness[i];
    }

    double total_fitness =
        cumulative.back();

    std::uniform_real_distribution<double>
        parent_dist(
            0.0,
            total_fitness);

    for(size_t i = 0; i < num_cells; i++)
    {
        double r = parent_dist(rng);

        auto it =
            std::lower_bound(
                cumulative.begin(),
                cumulative.end(),
                r);

        size_t parent =
            std::min(
                static_cast<size_t>(
                    std::distance(
                        cumulative.begin(),
                        it)),
                num_cells - 1);

        next_pop.genome[i] = pop.genome[parent];
        next_pop.age[i] = pop.age[parent] + 1;
        next_pop.passenger_mutations[i] =
            pop.passenger_mutations[parent];
    }
}