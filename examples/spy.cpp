#include "../matplotlibcpp.h"

#include <iostream>
#include <vector>

namespace plt = matplotlibcpp;

int main()
{
    const int n = 20;
    std::vector<std::vector<double>> matrix;

    for (int i = 0; i < n; ++i) {
        std::vector<double> row;
        for (int j = 0; j < n; ++j) {
            if (i == j)
                row.push_back(-2);
            else if (j == i - 1 || j == i + 1)
                row.push_back(1);
            else
                row.push_back(0);
        }
        matrix.push_back(row);
    }

    plt::spy(matrix, 5, {{"marker", "o"}});
    plt::show();

    return 0;
}
