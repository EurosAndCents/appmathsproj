#include <matplot/matplot.h>
#include <cmath>
#include <vector>

int main() {
    using namespace matplot;

    // Generate data
    std::vector<double> x = []() {
        std::vector<double> gen{};
        for (int i=0; i<1000; i++) {
            gen.push_back(i*0.001);
        }
        return gen;
    }();
    std::vector<double> y = [&]() {
        std::vector<double> gen{};
        for (auto i : x) {
            gen.push_back(std::sin(i*2*pi));
        }
        return gen;
    }();



    // Create plot
    plot(x, y);
    title("Population Model");
    xlabel("Years");
    ylabel("Population");

    // Display plot
    show();

    return 0;
}