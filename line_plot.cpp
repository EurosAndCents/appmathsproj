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
        for (int i=0; i<40*static_cast<int>(1.0f/constants::K_fidelity); i++) {
            // fidelity = 0.01, 1/fidelity=100, therefore i=100 = 1 year, going to 100 years
            this->x.push_back(static_cast<double>(i)*constants::K_fidelity); // Determines the "fidelity" of the graph
        }

        // Y values
        bool B_is_extinct{false};
        for (unsigned i=0; i<x.size(); i++) {
            if (!B_is_extinct) {
                current_birth_rate_ = constants::K_birth_rate; // Reset birth rate each iteration
                current_death_rate_ = constants::K_death_rate; // Reset death rate each iteration

                const double time{x[i]};

                // Yearly oscillation (to prevent a plateau)
                double season_factor = 0.1 * std::sin(time * 2 * M_PI);
                current_birth_rate_ *= (1.0 + season_factor);

                // Birth rate decreases at extremes, increases in the middle. Minimum reduction scalar is 0.5, maximum is 1.0.
                double gaussian_value{gaussian_distribution(population_/constants::K_carrying_capacity)};
                gaussian_value=std::clamp(gaussian_value-0.2,0.0,1.0);
                current_birth_rate_ *= gaussian_value;

                // std::lerp(constants::K_death_rate-0.3, constants::K_death_rate, population_/20000.0f)

                // Basic stochasticity - All bounds multiplied by 100 to convert double constants to integers for popmodel::bounded_rand()
                // EXPERIMENTAL - increasing the stochasticity of the death rate during hunting season. If removing this, re-enable hunting season above.
                const unsigned death_bound = static_cast<unsigned>(constants::K_death_bound +
                    ((time-std::floor(time) > constants::K_hunting_season_start)
                ? std::lerp( 0.0f, constants::K_death_bound_hunting,
                    (time-std::floor(time)-constants::K_hunting_season_start) / (1.0f-constants::K_hunting_season_start) ) // Normalizing the remaining time in the hunting season from 9/12=0 to 12/12=1
                : 0)
                )*100;
                constexpr unsigned birth_bound{static_cast<int>(constants::K_birth_bound*100)};

                // Gives noise in the range [-bound/2, +bound/2]
                const double death_rand_noise = (static_cast<double>(popmodel::bounded_rand(death_bound)) - death_bound/2) / 100;
                const double birth_rand_noise = (static_cast<double>(popmodel::bounded_rand(birth_bound)) - birth_bound/2) / 100;
                current_death_rate_ += death_rand_noise;
                current_birth_rate_ += birth_rand_noise;

                // P=Ce^(rt) or y(t)=Pe^[rt]
                effective_rate_ = current_birth_rate_ - current_death_rate_;
                er.push_back(effective_rate_);
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