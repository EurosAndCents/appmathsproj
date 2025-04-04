#pragma once

#include <cmath>
#include <random>
#include <string>
#include <__random/random_device.h>

namespace popmodel::constants {
    // Global constexpr variables
    constexpr float K_fidelity{0.01f}; // 1 year = 100x
    constexpr double K_init_population{135000}; // Estimations online say 120k-150k, so use a mean figure
    constexpr double K_carrying_capacity{400000}; // Carrying capacity limit due to food, land, and water

    // Some of these rates are updated below in lambda func y=[](){}, they are here for reference
    constexpr double K_birth_rate{0.4f}; // Assume 50% are female, and 50% giving birth each year
    constexpr double K_death_rate{0.2f}; // Assume 20% die per year - fluctuates due to hunting season (September-December @ 9/12-12/12)

    constexpr double K_hunting_season_start{9.5f/12.0f}; // Start in september, 9/12
    constexpr double K_death_rate_hunting{0.3f}; // Assume 30% die during hunting season (50k/135k)
    constexpr double K_death_bound_hunting{0.2f}; // Added to the current stochasticism of the death rate during hunting season (EXPERIMENTAL)

    constexpr double K_death_bound{0.1f}; // The bound of variability that can be added to the death rate
    constexpr double K_birth_bound{0.9f}; // The bound of variability that can be added to the birth rate
}

namespace popmodel {
    // Func taken from https://cppreference.com
    inline unsigned bounded_rand(const unsigned range) {
        static std::mt19937 gen(std::random_device{}()); // Seeded once
        std::uniform_int_distribution<unsigned> dist(0, range - 1);
        return range == 0 ? 0 : dist(gen);
    }

    inline double gaussian_distribution(const double& val) {
        // f(x) = (1/σsqr(2pi)) * e^-(x-μ)^2/2(2σ^2) -> log tables, normalized
        double standard_deviation{0.4}; // σ = 0.4
        double mean{constants::K_init_population/constants::K_carrying_capacity+0.1}; // μ = wherever the starting pop is
        double exponent = -std::pow(val - mean, 2) / (2 * std::pow(standard_deviation, 2));
        double gaussian_value = std::pow(M_E, exponent);

        // Normalize to range 0-1
        double min_gaussian_value = std::pow(M_E, -std::pow(mean - mean, 2) / (2 * std::pow(standard_deviation, 2)));
        double max_gaussian_value = std::pow(M_E, -std::pow(0 - mean, 2) / (2 * std::pow(standard_deviation, 2)));

        return 1 - (gaussian_value - min_gaussian_value) / (max_gaussian_value - min_gaussian_value);

    }
}
