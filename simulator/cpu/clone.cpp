#include "clone.hpp"

std::string genome_to_string(Genome genome)
{
    static const char* names[] =
    {
        "TP53",
        "KRAS",
        "EGFR",
        "BRCA1",
        "BRCA2",
        "APC",
        "PIK3CA",
        "PTEN"
    };

    std::string result;

    for(int i = 0; i < 8; i++)
    {
        if(genome & (1u << i))
        {
            if(!result.empty())
            {
                result += "|";
            }

            result += names[i];
        }
    }

    return result.empty()
           ? "Healthy"
           : result;
}