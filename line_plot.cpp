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
    population_(bounded_rand(500)+constants::K_init_population),
    colour_{bounded_rand(200)/255.0f, bounded_rand(200)/255.0f, bounded_rand(200)/255.0f} {
        this->generate_values();
    }

    // Functionality
    void line_plot::generate_values() {
        // X values
        for (int i=0; i<100*static_cast<int>(1.0f/constants::K_fidelity); i++) {
            // fidelity = 0.01, 1/fidelity=100, therefore i=100 = 1 year, going to 100 years
            this->x.push_back(static_cast<double>(i)*constants::K_fidelity); // Determines the "fidelity" of the graph
        }

        // Y values
        for (int i=0; i<x.size(); i++) {
            const double time{x[i]};

            // Hunting season is from September to December
            if (time-floor(time) > (8.0f/12.0f))
                // If in latter 4 months, hunting season, higher death rate
                current_death_rate_ = constants::K_death_rate_hunting;
            else
                // Not hunting season, normal death rate, but lerped as there is minimum food, land, and water
                current_death_rate_ = std::lerp(constants::K_death_rate-0.2f, constants::K_death_rate+0.2f, population_/constants::K_carrying_capacity);

            effective_rate_ = constants::K_birth_rate - current_death_rate_;
            population_ *= pow(M_E, (effective_rate_*constants::K_fidelity)); // P=Ce^(rt) or y(t)=Pe^[rt]

            this->y.push_back(population_);
        }
    }

    void line_plot::plot(const matplot::axes_handle& axes) const {
        axes->plot(this->x,this->y)->color(this->colour_);
    }
}