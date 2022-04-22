//
// g++ -g -Wall -o contour $(python-config --includes) contour.cpp $(python-config --ldflags --embed)
//

#include "../matplotlibcpp.h"

#include <cmath>

namespace plt = matplotlibcpp;

int main()
{
    std::vector<std::vector<double>> x, y, z;
    for (double i = -5; i <= 5;  i += 0.25) {
        std::vector<double> x_row, y_row, z_row;
        for (double j = -5; j <= 5; j += 0.25) {
            x_row.push_back(i);
            y_row.push_back(j);
            z_row.push_back(::std::sin(::std::hypot(i, j)));
        }
        x.push_back(x_row);
        y.push_back(y_row);
        z.push_back(z_row);
    }

    plt::contour(x, y, z);
    plt::show();

    plt::detail::_interpreter::kill();
    return (0);
}
