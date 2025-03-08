#include <matplot/matplot.h>
#include <cmath>
#include <vector>
#include <algorithm>

// Global constexpr variables
constexpr float K_fidelity{0.01f}; // 1 year = 1000x
constexpr double K_init_population{135000}; // Estimations online say 120k-150k, so use a mean figure
constexpr double K_carrying_capacity{400000}; // Carrying capacity limit due to food, land, and water

// Some of these rates are updated below in lambda func y=[](){}, they are here for reference
constexpr double K_birth_rate{0.5f*0.5f}; // Assume 50% are female, and 50% giving birth each year
constexpr double K_death_rate{0.2f}; // Assume 20% die per year - fluctuates due to hunting season (September-December @ 9/12-12/12)
constexpr double K_death_rate_hunting{0.3f}; // Assume 30% die during hunting season (50k/135k)

void generate_values(std::vector<double>& x, std::vector<double>& y) {
    using namespace std;

    // Variables
    double effective_rate{}; // Effective rate of population growth
    double current_birth_rate, current_death_rate{};

    double population{K_init_population}; // Ensures the model is recursive

    // Generate data
    for (int i=0; i<100*static_cast<int>(1/K_fidelity); i++) { // fidelity = 0.01, 1/fidelity=100, therefore i=100 = 1 year, going to 100 years
        x.push_back(i*K_fidelity); // Do not touch - determines the "fidelity" of the graph
    }

    for (int i=0; i<x.size(); i++) {
        const double time{x[i]};

        if (time-floor(time) > (8.0f/12.0f)) // Hunting season is from September to December
            current_death_rate = K_death_rate_hunting;
        else // Not hunting season, normal death rate, but lerped as there is minimum food, land, and water
            current_death_rate = lerp(K_death_rate-0.1f, K_death_rate+0.2f, population/400000.0f);

        effective_rate = K_birth_rate - current_death_rate;
        population *= pow(M_E, (effective_rate*K_fidelity)); // P=Ce^(rt)

        if (i!=0)
            cout << "Deaths within 0.001 years: " << population-y[i-1] << endl;

        y.push_back(population); // y=Pe^(rt)
    }
}

int main() {
    std::vector<double> x, y{};

    generate_values(x, y); // Reference params

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