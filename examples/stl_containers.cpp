#define _USE_MATH_DEFINES
#include <algorithm>
#include <cmath>
#include <deque>
#include <iostream>
#include <list>
#include "../matplotlibcpp.h"

namespace plt = matplotlibcpp;

double fun(double t)
{
    return 0.0981 * cos(0.1 * t - 0.007845) - exp(-0.3999 * t) * (0.0981 * cos(3.169 * t) + 0.0124 * sin(3.169 * t));
}

int main()
{
    /* ################################## Raw buffer ################################### */
    double t[500];
    double y[500];
    std::iota(t, t + 500, 0);
    std::transform(t, t + 500, t, [](double t) { return t / 50; });
    std::transform(t, t + 500, y, fun);

    plt::subplot(2, 1, 1);
    plt::title("Data stored in raw C-array");
    plt::stl::plot(t, t + 500, y, {{"linewidth", "3"}});

    /* ####################### Non-memory-contiguous containers ######################## */
    std::list<double> tlist{t, t + 500};
    std::deque<double> ydeque{y, y + 500};

    plt::subplot(2, 1, 2);
    plt::title("Data stored in std::list and std::double");
    plt::stl::plot<std::list<double>::iterator, std::deque<double>::iterator, plt::IS_NON_CONTIGUOUS,
                   plt::IS_NON_CONTIGUOUS>(tlist.begin(), tlist.end(), ydeque.begin(), "r--");
    plt::tight_layout();
    plt::show();
}