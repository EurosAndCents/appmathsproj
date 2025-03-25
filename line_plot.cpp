#include "line_plot.h"

#include <vector>
#include <cmath>
#include <iostream>
#include <matplot/matplot.h>
#include <array>

#include "constants.h"

namespace popmodel {
    // Constructor
    line_plot::line_plot() :
    effective_rate_(0.0f), current_birth_rate_(constants::K_birth_rate), current_death_rate_(constants::K_death_rate),
    population_(constants::K_init_population),
    colour_{static_cast<float>(bounded_rand(200))/255.0f, static_cast<float>(bounded_rand(200))/255.0f, static_cast<float>(bounded_rand(200))/255.0f} {
        this->generate_values();
    }

    // Functionality
    void line_plot::generate_values() {
        // X values
        for (int i=0; static_cast<float>(i)<100.0f*(1.0f/constants::K_fidelity); i++) { // fidelity = 0.01, 1/fidelity=100, therefore i=100 = 1 year, going to 100 years
            this->x.push_back(static_cast<double>(i)*constants::K_fidelity); // Determines the "fidelity" of the graph
        }

        // Y values
        bool B_is_extinct{false};
        for (unsigned i=0; i<x.size(); i++) {
            if (!B_is_extinct) {
                current_birth_rate_ = constants::K_birth_rate; // Reset birth rate each iteration
                current_death_rate_ = constants::K_death_rate; // Reset death rate each iteration

                const double time{x[i]};
                const double gaussian_value{gaussian_distribution(population_/10000.0f)};
                current_birth_rate_ *= std::clamp(gaussian_value,0.5,1.0); // Birth rate decreases at extremes, increases in the middle. Minimum reduction scalar is 0.5, maximum is 1.0.
                // std::lerp(constants::K_death_rate-0.3, constants::K_death_rate, population_/20000.0f)

                // Basic stochasticity
                // EXPERIMENTAL - increasing the stochasticity of the death rate during hunting season. If removing this, re-enable hunting season above.
                const unsigned death_bound = (time-std::floor(time) > constants::K_hunting_season_start)
                ? (constants::K_death_bound + std::lerp(0.0f, constants::K_death_hunting_bound, (time-std::floor(time)-constants::K_hunting_season_start)/(1.0f-constants::K_hunting_season_start)))*100
                : constants::K_death_bound*100;

                constexpr unsigned birth_bound{static_cast<int>(constants::K_birth_bound)*100};
                current_death_rate_ += static_cast<double>(popmodel::bounded_rand(death_bound)) / 100.0f;
                current_birth_rate_ += static_cast<double>(popmodel::bounded_rand(birth_bound)) / 100.0f;

                // P=Ce^(rt) or y=Pe^(rt)
                effective_rate_ = current_birth_rate_ - current_death_rate_;
                population_ *= pow(M_E, (effective_rate_*constants::K_fidelity));

                // Extinction check
                if (population_ <= 5) {
                    population_=0;
                    B_is_extinct=true;
                    std::cout << "Population extinct at year " << time << std::endl;
                }
            }

            this->y.push_back(population_);
        }
    }

    void line_plot::plot(const matplot::axes_handle& axes) const {
        axes->plot(this->x,this->y)->color(this->colour_);
    }
}