#include "line_plot.h"
#include <matplot/matplot.h>

#include <iostream>
#include <fstream>
#include <ctime>

#include "constants.h"

int main() {
    using namespace matplot;

    // Create plot
    auto fig = figure();
    auto axes = fig->current_axes();
    axes->hold(true);
    axes->title("Deer Population Model");
    axes->xlabel("Years");
    axes->ylabel("Population");

    // Generate lines
    std::vector<std::vector<double>> all_y_values;
    std::vector<double> x_axis;

    constexpr unsigned K_number_of_runs = 20;

    unsigned time_total{0};
    for (unsigned i=0, time, time_total_local; i<K_number_of_runs; i++) {
        std::cout << "Plotting line " << i+1 << std::endl;
        time = std::time(nullptr);

        // Create lines
        popmodel::line_plot line;

        if (i == 0) {
            x_axis = line.x; // Store x once
        }
        all_y_values.push_back(line.y);

        // Time debug info
        time_total_local = std::time(nullptr)-time;
        std::cout << "Line " << i+1 << " plotted";
        std::cout << " in " << time_total_local << "s" << std::endl;
        time_total += time_total_local;

        //axes->plot(line.x,line.er);

    }
    std::cout << "Completed line generation in " << time_total << "s" << std::endl;

    size_t num_points = x_axis.size();
    std::vector<double> mean(num_points);
    std::vector<double> lower_bound(num_points);
    std::vector<double> upper_bound(num_points);

    double Z = 1.96; // 95% confidence interval

    for (size_t t = 0; t < num_points; ++t) {
        std::vector<double> values_at_t(K_number_of_runs);
        for (size_t run = 0; run < K_number_of_runs; ++run) {
            values_at_t[run] = all_y_values[run][t];
        }

        // Mean
        double sum = std::accumulate(values_at_t.begin(), values_at_t.end(), 0.0);
        double mu = sum / values_at_t.size();

        // Standard deviation
        double sq_diff_sum = 0.0;
        for (double v : values_at_t) {
            sq_diff_sum += std::pow(v - mu, 2);
        }
        double sigma = std::sqrt(sq_diff_sum / values_at_t.size());

        double margin_of_error = Z * sigma / std::sqrt(values_at_t.size());

        mean[t] = mu;
        lower_bound[t] = mu - margin_of_error;
        upper_bound[t] = mu + margin_of_error;
    }

    // Create the filled area manually (with proper polygon ordering)
    std::vector<double> fill_x;
    std::vector<double> fill_y;
    size_t num = x_axis.size();

    // First, add the upper bound points (left to right)
    for (size_t i = 0; i < num; ++i) {
        fill_x.push_back(x_axis[i]);
        fill_y.push_back(upper_bound[i]);
    }

    // Then, add the lower bound points in reverse (right to left)
    for (size_t i = num; i-- > 0;) {
        fill_x.push_back(x_axis[i]);
        fill_y.push_back(lower_bound[i]);
    }

    // Now draw the polygon using fill()
    auto h = matplot::fill(fill_x, fill_y);
    h->color({0.1f, 0.2f, 0.5f, 0.3f}); // Example: light blue with 30% opacity


    // Display plot
    show();

    return 0;
}