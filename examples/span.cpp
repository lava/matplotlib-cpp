//
// g++ -std=c++20 -g -Wall -o span $(python-config --includes) span.cpp $(python-config --ldflags --embed)
//
#include "../matplotlibcpp.h"

#include <span>

using namespace std;
namespace plt = matplotlibcpp;

int main()
{
    // C-style arrays with multiple rows
    time_t t[]={1, 2, 3, 4};
    int data[]={
	3, 1, 4, 5,
	5, 4, 1, 3,
	3, 3, 3, 3
    };

    size_t n=sizeof(t)/sizeof(decltype(t[0]));
    size_t m=sizeof(data)/sizeof(decltype(data[0]));

    // Use std::span to pass data chunk to plot(), without copying it.
    // Unfortunately, plt::plot() makes an internal copy of both x and y
    // before passing them to python.
    for (size_t offset=0; offset<m; offset+=n)
	plt::plot(t, std::span {data+offset, n}, "");
    plt::show();

    plt::detail::_interpreter::kill();

    return 0;
}
