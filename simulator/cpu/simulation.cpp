#include "simulation.hpp"
#include "fitness.hpp"

#ifdef _OPENMP
#include<omp.h>
#endif

void compute_population_fitness(
    Population& pop)
{
    #pragma omp parallel for schedule(static)
    for(size_t i = 0;
        i < pop.genome.size();
        i++)
    {
        CellState state =
            calculate_cell_state(
                pop.genome[i],
                pop.age[i],
                pop.passenger_mutations[i]);

        pop.fitness[i] =
            calculate_fitness(state);
    }
}