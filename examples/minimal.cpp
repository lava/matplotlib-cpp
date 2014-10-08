#include "../matplotlibcpp.h"

namespace plt = matplotlibcpp;

int main() {
    std::vector<double> v {1,2,3,4};
    plt::plot(v);
    plt::show();
}
