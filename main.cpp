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
    unsigned time_total{0};
    for (unsigned i=0, time, time_total_local; i<20; i++) {
        std::cout << "Plotting line " << i+1 << std::endl;
        time = std::time(nullptr);

        // Create lines
        popmodel::line_plot line;
        line.popmodel::line_plot::plot(axes);

        // Time debug info
        time_total_local = std::time(nullptr)-time;
        std::cout << "Line " << i+1 << " plotted";
        std::cout << " in " << time_total_local << "s" << std::endl;
        time_total += time_total_local;

        //axes->plot(line.x,line.er);

    }
    std::cout << "Completed plot in " << time_total << "s" << std::endl;

    // Display plot
    show();

    return 0;
}