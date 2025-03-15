#pragma once

#include <cmath>

namespace popmodel::constants {
    // Global constexpr variables
    constexpr float K_fidelity{0.01f}; // 1 year = 100x
    constexpr double K_init_population{1000}; // Assume a deer population of 1000 (red deer, fallow deer, sika deer) -> each give birth to ~1 fawn per year

    // Some of these rates are updated below in lambda func y=[](){}, they are here for reference
    constexpr double K_birth_rate{0.5*0.5}; // Assume 50% are female, and 50% giving birth each year
    constexpr double K_death_rate{0.2}; // Assume 20% die per year - fluctuates due to hunting season (September-December @ 9/12-12/12)
    constexpr double K_death_rate_hunting{0.3}; // Assume 30% die during hunting season
}

namespace popmodel {
    // Func taken from cppreference.com
    inline float bounded_rand(unsigned range) {
        for (unsigned x, r;;) {
            if (x = std::rand(), r = x % range, x - r <= -range) {
                return static_cast<float>(r);
            }
        }
    }
}
