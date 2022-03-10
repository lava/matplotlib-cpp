//
// g++ -g -Wall -o minimal $(python-config --includes --cflags) minimal.cpp $(python-config --ldflags --embed)
//
#include "../matplotlibcpp.h"

namespace plt = matplotlibcpp;

int main() {
    plt::plot({1,3,2,4});
    plt::show();

    plt::detail::_interpreter::kill();
    return 0;
}
