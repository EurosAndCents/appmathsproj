#include <matplot/matplot.h>
#include <cmath>
#include <vector>
#include <algorithm>

int main() {
    using namespace std;

    // Variables
    constexpr double K_init_population{135000}; // Estimations online say 120k-150k, so use a mean figure
    constexpr double K_fidelity{0.01f}; // Fidelity of the graph

    // These rates are updated below in lambda func y=[](){}, they are here for reference
    double birth_rate{0.5f*0.5f}; // Assume 50% are female, and 50% giving birth each year
    double death_rate{0.2f}; // Assume 20% die per year - fluctuates due to hunting season (September-December @ 9/12-12/12)
    constexpr double K_death_rate_hunting{0.3f}; // Assume 30% die during hunting season (50k/135k)

    double effective_rate{birth_rate - death_rate}; // Effective rate of population growth
    double current_birth_rate, current_death_rate;

    double population{K_init_population}; // Ensures the model is recursive

    // Generate data
    vector<double> x = []() {
        vector<double> gen{};
        for (int i=0; i<100*static_cast<int>(1.0f/K_fidelity); i++) { // i=1000 = 1 year, going to 100 years
           gen.push_back(i*K_fidelity); // Do not touch - determines the "fidelity" of the graph
        }
        return gen;
    }();

    vector<double> y = [&]() {
        vector<double> gen{};
        for (int i=0; i<x.size(); i++) {
            const double time{x[i]};
            if (time-floor(time) > (8.0f/12.0f))
                current_death_rate = K_death_rate_hunting;
            else
                current_death_rate = lerp(death_rate-0.1f, death_rate+0.2f, population/400000.0f); // Not hunting season, normal death rate, but lerped as there is minimum food, land, and water

            effective_rate = birth_rate - current_death_rate;
            population *= pow(M_E, (effective_rate*K_fidelity)); // P=Ce^(rt)

            if (i!=0)
                std::cout << "Deaths within 0.01 years:" << population-gen[i-1] << std::endl;

            gen.push_back(population); // y=Pe^(rt)
        }
        return gen;
    }();


    using namespace matplot;

    // Create plot + datablock
    auto fig = figure();
    auto axes = fig->current_axes();
    axes->hold(true);
    axes->plot(x, y);
    axes->title("Deer Population Model");
    axes->xlabel("Years");
    axes->ylabel("Population");

    // Display plot
    show();

    return 0;
}