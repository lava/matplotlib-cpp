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

    plt::plot(span(t, 4), span(data, 12));
    plt::show();

    plt::detail::_interpreter::kill();

    return 0;
}
