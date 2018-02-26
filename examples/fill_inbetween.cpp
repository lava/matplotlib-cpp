#define _USE_MATH_DEFINES
#include "../matplotlibcpp.h"
#include <cmath>
#include <iostream>

using namespace std;
namespace plt = matplotlibcpp;

int main() {
  // Prepare data.
  int n = 5000;
  std::vector<double> x(n), y(n), z(n), w(n, 2);
  for (int i = 0; i < n; ++i) {
    x.at(i) = i * i;
    y.at(i) = sin(2 * M_PI * i / 360.0);
    z.at(i) = log(i);
  }

  // Prepare keywords to pass to PolyCollection. See
  // https://matplotlib.org/api/_as_gen/matplotlib.axes.Axes.fill_between.html
  std::map<string, string> keywords;
  keywords["alpha"] = "0.4";
  keywords["color"] = "grey";
  keywords["hatch"] = "-";

  plt::fill_between(x, y, z, keywords);
  plt::show();
}
