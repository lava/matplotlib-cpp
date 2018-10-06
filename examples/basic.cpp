#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include "../matplotlibcpp.h"

namespace plt = matplotlibcpp;

int main() 
{
    // Prepare data.
    int n = 5000;
    std::vector<double> x(n), y(n), z(n), w(n,2);
    for(int i=0; i<n; ++i) {
        x.at(i) = i*i;
        y.at(i) = sin(2*M_PI*i/360.0);
        z.at(i) = log(i);
    }
    
    // Set the size of output image = 1200x780 pixels
    plt::figure_size(1200, 780);

    // Plot line from given x and y data. Color is selected automatically.
    plt::plot(x, y);

    // Plot a red dashed line from given x and y data.
    plt::plot(x, w,"r--");

    // Plot a line whose name will show up as "log(x)" in the legend.
    plt::named_plot("log(x)", x, z);

    // Set x-axis to interval [0,1000000]
    plt::xlim(0, 1000*1000);

    // Add graph title
    plt::title("Sample figure");

    // Enable legend.
    plt::legend();

    // save figure
    const char* filename = "./basic.png";
    std::cout << "Saving result to " << filename << std::endl;;
    plt::save(filename);
}
