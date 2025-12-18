// Custom classes
#include "RooTDalitz/random.h"
#include "RooTDalitz/memory.h"

namespace Random{
    TRandom2& get_generator(){
        return Memory::Singleton<TRandom2>::getInstance();
    };

    void set_seed(unsigned long seed){
        TRandom2& generator = get_generator();
        generator.SetSeed(seed);
    }
} // namespace Memory
