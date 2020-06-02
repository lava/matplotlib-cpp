#include "../matplotlibcpp.h"

namespace plt = matplotlibcpp;

void minimal() {
	plt::plot( { 1, 3, 2, 4 });
	plt::show();
}
