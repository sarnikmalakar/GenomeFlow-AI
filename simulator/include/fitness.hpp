#pragma once

#include <cstdint>

#include "genome.hpp"

struct CellState
{
    // Probability of cell division
    float division_probability = 1.0f;

    // Probability of apoptosis
    float death_probability = 1.0f;

    // Relative mutation rate
    float mutation_rate = 1.0f;

    // DNA repair efficiency
    float dna_repair = 1.0f;
};

// Compute biological state of a cell
CellState calculate_cell_state(
    Genome genome,
    uint16_t age,
    uint16_t passenger_mutations);

// Convert CellState into evolutionary fitness
float calculate_fitness(
    const CellState& state);