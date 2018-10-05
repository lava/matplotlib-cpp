#define _USE_MATH_DEFINES
#include <cmath>
#include "../matplotlibcpp.h"

using namespace std;
namespace plt = matplotlibcpp;

int main() 
{
    // Prepare data
	int n = 500;
	std::vector<double> x(n), y(n), z(n), w(n,2);
	for(int i=0; i<n; ++i) {
		x.at(i) = i;
		y.at(i) = sin(2*M_PI*i/360.0);
		z.at(i) = 100.0 / i;
	}

    // Set the "super title"
    plt::suptitle("My plot");
    plt::subplot(1, 2, 1);
	plt::plot(x, y, "r-");
    plt::subplot(1, 2, 2);
    plt::plot(x, z, "k-");
    // Add some text to the plot
    plt::text(100, 90, "Hello!");


	// Show plots
	plt::show();
}
