#define _USE_MATH_DEFINES
#include <cmath>
#include "../matplotlibcpp.h"
#include <chrono>

namespace plt = matplotlibcpp;

void update_window(const double x, const double y, const double t,
                   std::vector<double> &xt, std::vector<double> &yt)
{
    const double target_length = 300;
    const double half_win = (target_length/(2.*sqrt(1.+t*t)));

    xt[0] = x - half_win;
    xt[1] = x + half_win;
    yt[0] = y - half_win*t;
    yt[1] = y + half_win*t;
}


int main()
{

    bool use_dynamic_plot = false;
    bool timeit = true;

    size_t n = 1000;
    std::vector<double> x, y;

    const double w = 0.05;
    const double a = n/2;

    for(size_t i=0; i<n; i++) {
        x.push_back(i);
        y.push_back(a*sin(w*i));
    }

    std::vector<double> xt(2), yt(2);

    plt::title("Sample figure");

    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    if(use_dynamic_plot)
    {
        plt::xlim(x.front(), x.back());
        plt::ylim(-a,a);
        plt::axis("equal");

        // plot sin once and for all
        plt::named_plot("sin", x, y);

        // prepare plotting the tangent
        plt::Plot plot("tangent");

        plt::legend();

        for(size_t i=0; i<n; i++) {

            if (i % 10 == 0) {
                {
                    update_window(x[i], y[i], a*w*cos(w*x[i]), xt, yt);
                    // just update data for this plot
                    plot.update(xt, yt);
                }

                // Display plot continuously
                if(!timeit)
                    plt::pause(0.1);
            }
        }
    }

    else
    {
        for(size_t i=0; i<n; i++) {

            if (i % 10 == 0) {
                {
                    plt::clf();

                    plt::named_plot("sin", x, y);

                    update_window(x[i], y[i], a*w*cos(w*i), xt, yt);
                    plt::named_plot("tangent", xt, yt);

                    // we have to control axis size
                    plt::xlim(x.front(), x.back());
                    plt::ylim(-a,a);
                    plt::axis("equal");
                    plt::legend();

                }

                // Display plot continuously
                if(!timeit)
                    plt::pause(0.1);
            }
        }
    }

    end = std::chrono::system_clock::now();
    double elapsed_seconds = std::chrono::duration_cast<std::chrono::microseconds>
            (end-start).count();
    if(use_dynamic_plot)
        std::cout << "dynamic";
    else
        std::cout << "static";

    std::cout << " : " << elapsed_seconds/1000 << " ms\n";
}
