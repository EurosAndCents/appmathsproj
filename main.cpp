#include <matplot/matplot.h>
#include <cmath>
#include <vector>

int main() {
    using namespace std;

    // Variables
    constexpr double init_population{1000}; // Assume a deer population of 1000 (red deer, fallow deer, sika deer) -> each give birth to ~1 fawn per year

    // These rates are updated below in lambda func y=[](){}, they are here for reference
    double birth_rate{0.5*0.5}; // Assume 50% are female, and 50% giving birth each year
    double death_rate{0.2}; // Assume 20% die per year - fluctuates due to hunting season (September-December @ 9/12-12/12)
    double effective_rate{birth_rate-death_rate}; // Effective rate of population growth

    // Generate data
    vector<double> x = []() {
        vector<double> gen{};
        for (int i=0; i<100000; i++) { // i=1000 = 1 year, going to 100 years
            gen.push_back(i*0.001); // Do not touch - determines the "fidelity" of the graph
        }
        return gen;
    }();

    vector<double> y = [&]() {
        vector<double> gen{};
        for (double time : x) {
            if (fmod(time, 1.0f) > (9/12)) {
                cout << "Hunting season @ " << time << " years" << endl;
            }
            effective_rate = birth_rate - death_rate;
            double population = init_population * pow(M_E, (effective_rate*time)); // P=Ce^(rt)

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