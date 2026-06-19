#pragma once
#include<cstdint>
enum class Gene: uint8_t
{
    TP53 = 0,
    KRAS,
    EGFR,
    BRCA1,
    BRCA2,
    APC,
    PIK3CA,
    PTEN,

    COUNT
};

using Genome = uint16_t;