#include <cmath>
#include<algorithm>

#include "fitness.hpp"

CellState calculate_cell_state(Genome genome,uint16_t age,uint16_t passenger_mutations)
{
    CellState state;

// -------------------------
// TP53
// Tumor suppressor
// Loss -> reduced apoptosis,
// poorer DNA repair,
// increased genomic instability
// -------------------------
    if(genome & (1u << static_cast<int>(Gene::TP53)))
    {
        state.death_probability *= 0.70f;
        state.dna_repair *= 0.50f;
        state.mutation_rate *= 1.30f;
    }

// -------------------------
// KRAS
// Oncogene
// Drives proliferation
// -------------------------
    if(genome & (1u << static_cast<int>(Gene::KRAS)))
    {
        state.division_probability *= 1.35f;
    }

// -------------------------
// EGFR
// Growth signalling receptor
// -------------------------
    if(genome & (1u << static_cast<int>(Gene::EGFR)))
    {
        state.division_probability *= 1.25f;
    }

// -------------------------
// BRCA1
// DNA repair gene
// -------------------------
    if(genome & (1u << static_cast<int>(Gene::BRCA1)))
    {
        state.dna_repair *= 0.60f;
        state.mutation_rate *= 1.50f;
    }

// -------------------------
// BRCA2
// DNA repair gene
// -------------------------
    if(genome & (1u << static_cast<int>(Gene::BRCA2)))
    {
        state.dna_repair *= 0.60f;
        state.mutation_rate *= 1.50f;
    }

// -------------------------
// APC
// Loss of growth suppression
// -------------------------
    if(genome & (1u << static_cast<int>(Gene::APC)))
    {
        state.division_probability *= 1.20f;
    }

// -------------------------
// PTEN
// Loss of growth inhibition
// -------------------------
    if(genome & (1u << static_cast<int>(Gene::PTEN)))
    {
        state.division_probability *= 1.15f;
        state.death_probability *= 0.85f;
    }

// -------------------------
// PIK3CA
// PI3K signalling activation
// -------------------------
    if(genome & (1u << static_cast<int>(Gene::PIK3CA)))
    {
        state.division_probability *= 1.20f;
        state.death_probability *= 0.90f;
    }

// -------------------------
// Epistasis
// -------------------------

    bool tp53 =
        genome & (1u << static_cast<int>(Gene::TP53));

    bool kras =
        genome & (1u << static_cast<int>(Gene::KRAS));

    bool egfr =
        genome & (1u << static_cast<int>(Gene::EGFR));

    bool brca1 =
        genome & (1u << static_cast<int>(Gene::BRCA1));

    bool brca2 =
        genome & (1u << static_cast<int>(Gene::BRCA2));

    bool apc =
        genome & (1u << static_cast<int>(Gene::APC));

    bool pik3ca =
        genome & (1u << static_cast<int>(Gene::PIK3CA));

    bool pten =
        genome & (1u << static_cast<int>(Gene::PTEN));

    if(tp53 && kras)
    {
        state.division_probability *= 1.20f;
        state.death_probability *= 0.90f;
    }

    if(kras && egfr)
    {
        state.division_probability *= 1.15f;
    }

    // TP53 + PTEN
    if(tp53 && pten)
    {
        state.death_probability *= 0.90f;
    }

    // TP53 + BRCA1
    if(tp53 && brca1)
    {
        state.mutation_rate *= 1.20f;
    }

    // TP53 + BRCA2
    if(tp53 && brca2)
    {
        state.mutation_rate *= 1.20f;
    }

    // PTEN + PIK3CA
    if(pten && pik3ca)
    {
        state.division_probability *= 1.15f;
    }

    // APC + KRAS
    if(apc && kras)
    {
        state.division_probability *= 1.10f;
    }

    if(tp53 && kras && egfr)
    {
        state.division_probability *= 1.25f;
        state.death_probability *= 0.85f;
        state.mutation_rate *= 1.10f;
    }

// -------------------------
// Cellular Senescence
// -------------------------

    float aging_factor =
        std::exp(-0.001f * age);

    // TP53 mutants partially escape senescence
    if(tp53)
    {
        aging_factor =
            std::exp(-0.0003f * age);
    }

    state.division_probability *= aging_factor;

    //Age-dependent Apoptosis
    if(tp53)
    {
        state.death_probability *= (1.0f + 0.0002f * age);
    }
    else
    {
        state.death_probability *= (1.0f + 0.0005f * age);
    }


// -------------------------
// Passenger Mutation Burden
// -------------------------

    if(passenger_mutations > 20)
    {
        state.division_probability *= 0.995f;
    }

    if(passenger_mutations > 50)
    {
        state.division_probability *= 0.98f;
        state.mutation_rate *= 1.05f;
    }

    if(passenger_mutations > 100)
    {
        state.death_probability *= 1.05f;
    }

// -------------------------
// Numerical Stability
// -------------------------

    state.division_probability =
        std::clamp(state.division_probability, 0.0f, 5.0f);

    state.death_probability =
        std::clamp(state.death_probability, 0.05f, 2.0f);

    state.mutation_rate =
        std::clamp(state.mutation_rate, 0.1f, 10.0f);

    state.dna_repair =
        std::clamp(state.dna_repair, 0.1f, 2.0f);

    return state;
    }

    float calculate_fitness(const CellState& state)
    {
        float fitness = 1.0f;

        fitness *= state.division_probability;

        fitness *= (2.0f - state.death_probability);

        fitness *= (2.0f - state.dna_repair) * 0.30f + 1.0f;

        fitness *= (state.mutation_rate - 1.0f) * 0.10f + 1.0f;

    return fitness;
    }   