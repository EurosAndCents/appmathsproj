#pragma once

#include <vector>
#include <matplot/matplot.h>
#include <array>

namespace popmodel {

    class line_plot {
    public:
        line_plot();

        // Member variables
        std::vector<double> x, // Time (years, each value is 0.01 (K_fidelity) years)
        y, // Current population of deer
        er; // Effective rate, for debugging purposes

        // Functionality
        void plot(const matplot::axes_handle& axes) const; // Plot the line graph, requires axes_handle to plot on
    private:
        // Functionality
        void generate_values();

        // Member variables
        double effective_rate_; // Effective rate of population growth (birth rate - death rate)
        double current_birth_rate_, current_death_rate_;

        double population_; // For calculation of the population of deer, initialized to K_init_population to ensure model is recursive

        std::array<float, 3> colour_; // RGB colour

    };

}