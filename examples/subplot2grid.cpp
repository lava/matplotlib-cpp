#define _USE_MATH_DEFINES
#include <cmath>
#include "../matplotlibcpp.h"

using namespace std;
namespace plt = matplotlibcpp;

int main()
{
    // Prepare data
	int n = 500;
	std::vector<double> x(n), u(n), v(n), w(n);
	for(int i=0; i<n; ++i) {
		x.at(i) = i;
		u.at(i) = sin(2*M_PI*i/500.0);
		v.at(i) = 100.0 / i;
		w.at(i) = sin(2*M_PI*i/1000.0);
	}

    // Set the "super title"
    plt::suptitle("My plot");

    const long nrows=3, ncols=3;
    long row = 2, col = 2;

    plt::subplot2grid(nrows, ncols, row, col);
	plt::plot(x, w, "g-");

    long spanr = 1, spanc = 2;
    col = 0;
    plt::subplot2grid(nrows, ncols, row, col, spanr, spanc);
	plt::plot(x, v, "r-");

    spanr = 2, spanc = 3;
    row = 0, col = 0;
    plt::subplot2grid(nrows, ncols, row, col, spanr, spanc);
    plt::plot(x, u, "b-");
    // Add some text to the plot
    plt::text(100., -0.5, "Hello!");


    // Show plots
	plt::show();
}
