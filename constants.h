#pragma once

#include <cmath>

namespace popmodel::constants {
    // Global constexpr variables
    constexpr float K_fidelity{0.01f}; // 1 year = 100x
    constexpr double K_init_population{135000}; // Estimations online say 120k-150k, so use a mean figure
    constexpr double K_carrying_capacity{400000}; // Carrying capacity limit due to food, land, and water

    // Some of these rates are updated below in lambda func y=[](){}, they are here for reference
    constexpr double K_birth_rate{0.5f*0.5f}; // Assume 50% are female, and 50% giving birth each year
    constexpr double K_death_rate{0.2f}; // Assume 20% die per year - fluctuates due to hunting season (September-December @ 9/12-12/12)
    constexpr double K_death_rate_hunting{0.3f}; // Assume 30% die during hunting season (50k/135k)
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
