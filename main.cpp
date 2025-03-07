#include <matplot/matplot.h>
#include <cmath>
#include <vector>
#include <algorithm>

int main() {
    using namespace std;

    // Variables
    constexpr float K_fidelity{0.01f}; // 1 year = 1000x
    constexpr double init_population{1000}; // Assume a deer population of 1000 (red deer, fallow deer, sika deer) -> each give birth to ~1 fawn per year

    // Some of these rates are updated below in lambda func y=[](){}, they are here for reference
    constexpr double K_birth_rate{0.5*0.5}; // Assume 50% are female, and 50% giving birth each year
    constexpr double K_death_rate{0.2}; // Assume 20% die per year - fluctuates due to hunting season (September-December @ 9/12-12/12)
    constexpr double K_death_rate_hunting{0.3}; // Assume 30% die during hunting season
    double effective_rate{K_birth_rate-K_death_rate}; // Effective rate of population growth
    double current_birth_rate, current_death_rate;

    double population{init_population}; // Ensures the model is recursive

    // Generate data
    vector<double> x = []() {
        vector<double> gen{};
        for (int i=0; i<50*(1/K_fidelity); i++) { // fidelity = 0.001, 1/fidelity=1000, therefore i=1000 = 1 year, going to 50 years
            gen.push_back(i*K_fidelity); // Do not touch - determines the "fidelity" of the graph
        }
        return gen;
    }();

    vector<double> y = [&]() {
        vector<double> gen{};
        for (int i=0; i<x.size(); i++) {
            double time{x[i]};
            // Hunting season is from September to December
            if (time-floor(time) > (8.0f/12.0f)) current_death_rate = K_death_rate_hunting;
            else current_death_rate = lerp(K_death_rate-0.1, K_death_rate+0.2, population/10000); // Not hunting season, normal death rate, but lerped as there is minimum food, land, and water

            effective_rate = K_birth_rate - current_death_rate;
            population *= pow(M_E, (effective_rate*K_fidelity)); // P=Ce^(rt)

            if (i!=0) cout << "Deaths within 0.001 years: " << population-gen[i-1] << endl;

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