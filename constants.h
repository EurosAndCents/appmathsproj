#pragma once

#include <cmath>
#include <string>

namespace popmodel::constants {
    // Global constexpr variables
    constexpr float K_fidelity{0.01f}; // 1 year = 100x
    constexpr double K_init_population{5000}; // Assume a deer population of 1000 (red deer, fallow deer, sika deer) -> each give birth to ~1 fawn per year

    // Some of these rates are updated below in lambda func y=[](){}, they are here for reference
    constexpr double K_birth_rate{0.55}; // Assume 50% are female, and 50% giving birth each year, inflated by +0.1 to account for gaussian distribution
    constexpr double K_birth_bound{0.6}; // The bound of variability that can be added to the birth rate

    constexpr double K_death_rate{0.3}; // Assume 30% die per year - fluctuates due to hunting season (September-December @ 9/12-12/12)
    constexpr double K_death_bound{0.45}; // The bound of variability that can be added to the death rate

    constexpr float K_hunting_season_start{9.5f/12.0f}; // Start in september
    constexpr double K_death_rate_hunting{0.3}; // Assume 30% die during hunting season
    constexpr double K_death_hunting_bound{0.3}; // Added to the current stochasticism of the death rate during hunting season (EXPERIMENTAL) // Assume 15% extra can die during hunting season
}

namespace popmodel {
    // Func taken from https://cppreference.com
    inline unsigned bounded_rand(const unsigned range) {
        if (range == 0) return 0; // Prevent division by zero
        for (unsigned x, r;;)
            if (x = std::rand(), r = x % range, x - r <= -range)
                return r;
    }

    inline double gaussian_distribution(const double& val) {
        // f(x) = (1/σsqr(2pi)) * e^-(x-μ)^2/2(2σ^2) -> log tables, normalized
        double standard_deviation{0.35}; // σ = 0.26
        double mean{0.5}; // μ = 0.5
        double exponent = -std::pow(val - mean, 2) / (2 * std::pow(standard_deviation, 2));
        double gaussian_value = std::pow(M_E, exponent);

        // Normalize to range 0-1
        double min_gaussian_value = std::pow(M_E, -std::pow(mean - mean, 2) / (2 * std::pow(standard_deviation, 2)));
        double max_gaussian_value = std::pow(M_E, -std::pow(0 - mean, 2) / (2 * std::pow(standard_deviation, 2)));

        return 1 - (gaussian_value - min_gaussian_value) / (max_gaussian_value - min_gaussian_value);

    }
}
