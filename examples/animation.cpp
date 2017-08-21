#define _USE_MATH_DEFINES
#include <cmath>
#include "../matplotlibcpp.h"

namespace plt = matplotlibcpp;

int main()
{
	int n = 1000;
	std::vector<double> x, y, z;

	for(int i=0; i<n; i++) {
		x.push_back(i*i);
		y.push_back(sin(2*M_PI*i/360.0));
		z.push_back(log(i));

		if (i % 10 == 0) {
			// Clear previous plot
			plt::clf();
			// Plot line from given x and y data. Color is selected automatically.
			plt::plot(x, y);
			// Plot a line whose name will show up as "log(x)" in the legend.
			plt::named_plot("log(x)", x, z);

			// Set x-axis to interval [0,1000000]
			plt::xlim(0, n*n);

			// Add graph title
			plt::title("Sample figure");
			// Enable legend.
			plt::legend();
			// Display plot continuously
			plt::pause(0.01);
		}
	}
}
