#include "line_plot.h"
#include <matplot/matplot.h>

#include <iostream>
#include <fstream>
#include <ctime>

int main() {
    using namespace matplot;

    // Create plot + datablock
    auto fig = figure();
    auto axes = fig->current_axes();
    axes->hold(true);
    axes->title("Deer Population Model");
    axes->xlabel("Years");
    axes->ylabel("Population");

    // Create and plot 20 line_plot objs
    unsigned time_total{0};
    for (unsigned i=0, time, time_total_local; i<20; i++) {
        std::cout << "Plotting line " << i+1 << std::endl;
        time = std::time(nullptr);

        popmodel::line_plot line;
        line.popmodel::line_plot::plot(axes);

        time_total_local = std::time(nullptr)-time;
        std::cout << "Line " << i+1 << " plotted";
        std::cout << " in " << time_total_local << "s" << std::endl;
        time_total += time_total_local;
    }
    std::cout << "Completed plot in " << time_total << "s" << std::endl;

    // Display plot
    show();

    return 0;
}