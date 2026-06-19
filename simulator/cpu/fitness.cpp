#include "fitness.hpp"

float calculate_fitness(Genome genome)
{
    float fitness = 1.0f;

    if(genome & (1 << static_cast<int>(Gene::TP53)))
        fitness += 0.20f;

    if(genome & (1 << static_cast<int>(Gene::KRAS)))
        fitness += 0.30f;

    if(genome & (1 << static_cast<int>(Gene::EGFR)))
        fitness += 0.25f;

    if(genome & (1 << static_cast<int>(Gene::BRCA1)))
        fitness += 0.10f;

    if(genome & (1 << static_cast<int>(Gene::BRCA2)))
        fitness += 0.10f;

    if(genome & (1 << static_cast<int>(Gene::APC)))
        fitness += 0.15f;

    if(genome & (1 << static_cast<int>(Gene::PIK3CA)))
        fitness += 0.18f;

    if(genome & (1 << static_cast<int>(Gene::PTEN)))
        fitness += 0.20f; 

    bool tp53 =
    (genome & (1 << static_cast<int>(Gene::TP53)))!=0;

    bool kras =
    (genome & (1 << static_cast<int>(Gene::KRAS)))!=0;

    bool egfr =
    (genome & (1 << static_cast<int>(Gene::EGFR)))!=0;

    if(tp53 && kras)
    {
        fitness += 0.50f;
    }

    if(kras && egfr)
    {
        fitness += 0.40f;
    }

    if(tp53 && kras && egfr)
    {
        fitness += 0.60f;
    }

    return fitness;
}