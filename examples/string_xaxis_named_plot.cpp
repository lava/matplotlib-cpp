#include "../matplotlibcpp.h"
#include <string>
#include <vector>

namespace plt = matplotlibcpp;

int main()
{
    const std::vector<std::string> months = {
        "Jan 2021", "Feb 2021", "Mar 2021", "Apr 2021", "May 2021"
    };
    const std::vector<double> speed_kmh = {58.4, 61.2, 63.9, 62.1, 65.0};

    plt::figure_size(900, 500);
    plt::named_plot("Vehicle speed (max)", months, speed_kmh, "o-");
    plt::title("Monthly peak speed");
    plt::xlabel("Month");
    plt::ylabel("Speed (km/h)");
    plt::legend();
    plt::tight_layout();

    // Use save() for CI/headless usage; replace with show() for interactive runs.
    plt::save("./string_xaxis_named_plot.png");
    return 0;
}
