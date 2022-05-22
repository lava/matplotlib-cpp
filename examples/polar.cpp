#include "../matplotlibcpp.h"

namespace plt = matplotlibcpp;

int main() {
    std::vector<double> r, th;
    for (int i = 0; i < 201; i++) {
        r.push_back(0 + 0.01 * i);
        th.push_back(2 * M_PI * r.back());
    }
    plt::polar(th, r, {{"color", "red"}});
    std::vector<double> ticks {0.5, 0.75, 1.0};
    plt::set_rticks(ticks);
    plt::set_rmax(2.0);
    plt::set_rlabel_position(90.0);
    plt::title("Polar plot");
    plt::show();
}
