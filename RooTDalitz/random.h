#pragma once

#include "TRandom2.h"

namespace Random{
    TRandom2& get_generator();
    void set_seed(unsigned long seed);
} // namespace Memory
