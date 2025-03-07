#include <matplot/matplot.h>
#include <cmath>
#include <vector>

int main() {
    using namespace matplot;

    // Variables
    const double init_population{1000}; // Assume a deer population of 1000 (red deer, fallow deer, sika deer) -> each give birth to ~1 fawn per year

    const double birth_rate{0.5*0.5}; // Assume 50% are female, and 50% giving birth each year
    const double death_rate{0.2}; // Assume 20% die per year
    const double effective_rate{birth_rate-death_rate}; // Effective rate of population growth

    // Generate data
    std::vector<double> x = []() {
        std::vector<double> gen{};
        for (int i=0; i<100000; i++) { // i=1000 = 1 year, going to 100 years
            gen.push_back(i*0.001); // Do not touch - determines the "fidelity" of the graph
        }
        return gen;
    }();
    std::vector<double> y = [&]() {
        std::vector<double> gen{};
        for (int i=0; i<x.size(); i++) {
            const double time = x[i];
            double population = init_population * pow(M_E, (effective_rate*time)); // P=Ce^(rt)

            gen.push_back(population); // y=Pe^(rt)
        }
        return gen;
    }();



    // Create plot
    plot(x, y);
    title("Deer Population Model");
    xlabel("Years");
    ylabel("Population");

    // Display plot
    show();

    return 0;
}