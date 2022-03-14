//
// g++ -std=c++20 -g -Wall -o ranges $(python-config --includes) ranges.cpp $(python-config --ldflags --embed)
//
// g++ -std=c++17 -g -Wall -o ranges $(python-config --includes) ranges.cpp $(python-config --ldflags --embed)
//
#include "../matplotlibcpp.h"

#include <iostream>
#include <list>
#include <map>
#include <span>
#include <string>
#include <vector>

#define UNUSED(x) (void)(x)

using namespace std;
namespace plt = matplotlibcpp;

int main()
{
    plt::detail::_interpreter::get();

    // C-style arrays with multiple rows.

#if __cplusplus >= CPP20

    time_t t[]={1, 2, 3, 4};

    // Care with column-major vs row-major!
    // C and Python are row-major, but usually a time series is column-major
    // and we want to plot the columns.
    // In the example below, these columns are [3,1,4,5] and [5,4,1,3], so
    // the data must be stored like this:
    double data [] = {
	3, 1, 4, 5,
	5, 4, 1, 3
    };             // contiguous data, column major!

    // Use std::span() to convert to a contiguous range (O(1)).
    // Data won't be copied, but passed as a pointer to Python.
    plt::plot(span(t, 4), span(data, 8));
    plt::grid(true);
    plt::title("C-arrays, with multiple columns");
    plt::show();

#else

    cerr << __FILE__ << ": " << __FUNCTION__ << ": " << __LINE__ << ": "
	 << "No support for C-style arrays in C++ <= 17" << endl;

#endif

    // vectors

    // Vectors are also contiguous ranges.
    // In C++20, as with span, plot resolves to plot(contiguous_range).
    // In C++ < 20 plot resolves to plot(vector).
    vector<double> x={1, 2, 3, 4, 5};
    vector<double> y={0, 1, 0, -1, 0};
    plt::plot(x, y);
    plt::grid(true);
    plt::title("vectors");
    plt::show();


    // lists

    // By contrast, lists are non-contiguous (but iterable) containers, so
    // plot resolves to plot(iterable).
    list<double> u { 1, 2, 3, 4, 5};
    list<double> v { 0, -1, 0, 1, 0};
    plt::plot(u, v, "");
    plt::grid(true);
    plt::title("Lists (non-contiguous ranges)");
    plt::show();


    // All together now
#if __cplusplus >= CPP20

    // If span is not last, plot resolves to plot(iterable), which copies data.
    // That's because in the dispatch plot() we have plot_impl() && plot()
    // (i.e. plot_impl() comes first), and we only have iterable and
    // callable plot_impl(). That sends us to the iterable plot_impl(),
    // rather than to plot(contiguous_range).
    //
    // TODO: have 3 tags plot_impl(): iterable, callable and contiguous range.
    plt::plot(span(t, 4), span(data, 8), "", x, y, "b", u, v, "r");
    plt::grid(true);
    plt::title("Variadic templates recursion, span first (copy)");
    plt::show();

    // This resolves to plot(contiguous_range) and does not copy data.
    plt::plot(x, y, "b", u, v, "r", span(t, 4), span(data, 8));
    plt::grid(true);
    plt::title("Variadic templates recursion, span last (passthrough)");
    plt::show();

#else

    // no C-arrays
    cerr << __FILE__ << ": " << __FUNCTION__ << ": " << __LINE__ << ": "
	 << "No support for C-style arrays in C++ <= 17" << endl;

    plt::plot(x, y, "b", u, v, "r");
    plt::grid(true);
    plt::title("Variadic templates recursion");
    plt::show();

#endif

    plt::detail::_interpreter::kill();

    return 0;
}
