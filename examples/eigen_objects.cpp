#define _USE_MATH_DEFINES

#include <Eigen/Core>
#include "../matplotlibcpp.h"

namespace plt = matplotlibcpp;

Eigen::ArrayXd fun(const Eigen::ArrayXd& t)
{
    using Eigen::cos;
    using Eigen::exp;
    using Eigen::sin;
    return 0.0981 * cos(0.1 * t - 0.007845) - exp(-0.3999 * t) * (0.0981 * cos(3.169 * t) + 0.0124 * sin(3.169 * t));
}

int main()
{
    Eigen::VectorXd x = Eigen::VectorXd::LinSpaced(100, 0, 10);
    Eigen::VectorXd y = fun(x.array());
    plt::stl::plot(x.data(), x.data() + x.size(), y.data(),
                   {{"label", "Eigen VectorXd"}, {"marker", "o"}, {"markersize", "5"}, {"markerfacecolor", "#3efab0"}});
    plt::legend();
    plt::show();
}