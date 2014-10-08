#include "../matplotlibcpp.h"

namespace plt = matplotlibcpp;

int main() 
{
	// plot(y) - the x-coordinates are implicitly set to [0,1,...,n)
	plt::plot({1,2,3,1,3.5,2.5}); 
	// plot(x,y,format) - plot as solid red line with circular markers
	plt::plot({5,4,3,2,-1}, {-1, 4, 2, 7, 1}, "ro-");

	// show plots
	plt::show();
}
