///
/// tu_matplotlibcpp.cpp
///
/// \brief  Unit Tests for matplotlibcpp.h
/// \date   03/22/2020
/// \author Claire Guilbaud

#ifdef CATCH_UNIT_TESTS
#include "catch.hpp"

#include <cmath>
#include <iterator>
#include <map>
#include <string>
#include <vector>

#if __cplusplus >= 201402L
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#endif

#include "matplotlibcpp.h"

using namespace std;

namespace plt = matplotlibcpp;

auto interval = 0.001;

// NOTES
// -----
// We do not test some methods because there are used in a lot of tests. So
// we already know if something goes wrong:
//      PyObject* get_array(const std::vector<Numeric>& v)
//      void clf()
//      void ion()
//      void show(true)
//      void add_side(path_as_string)
//
//
// WARNING
// -------
// * If there is a segmentation violation, it could be revealed only at the next
// call of plt::clf(), or rarely in then matplotlibcpp's function call.
// * functions in matplotlibcpp do some `Py_DECREF` on args objects parameters.
// So for numpy C arrays, created by `get_array(int, Numeric*)`, do not call
// `Py_DECREF` otherwise you could have plantage in matplotlibcpp calls.

TEST_CASE("exists", "[interpreter][utilities]") {
#if __cplusplus >= 201402L
  SECTION("exists") {
    const fs::path this_file(__FILE__);
    REQUIRE(plt::detail::_interpreter::exists(this_file));
    REQUIRE(!plt::detail::_interpreter::exists("/toto"));
  }
#endif // __cplusplus >= 201402L
}

TEST_CASE("utilities", "[interpreter][utilities]") {
  // /!\ Essential for testing on Ubuntu OS because Tk is not installed by
  // default. Otherwise some tests failed.
  plt::backend("Qt5Agg");
  plt::ion();

  SECTION("add_site") {
    // irrelevant directory
    REQUIRE_NOTHROW(plt::detail::_interpreter::add_site("/usr/local"));
  }

  SECTION("append_sys_path") {
    // irrelevant directory
    REQUIRE_NOTHROW(plt::detail::_interpreter::append_sys_path("/usr"));
  }

  SECTION("print_sys_path") {
    REQUIRE_NOTHROW(plt::detail::_interpreter::print_sys_path());
  }
}

TEST_CASE("matplotlib", "[matplotlibcpp][matplotlib]") {
  // /!\ Essential for testing on Ubuntu OS because Tk is not installed by
  // default.
  plt::backend("Qt5Agg");

  // Some data
  unsigned long n = 5000;
  std::vector<double> x(n), y(n), z(n), w(n, 2);
  for (unsigned long i = 0; i < n; ++i) {
    auto j = i + 1;
    x.at(i) = j * j;
    y.at(i) = 1 + sin(2 * M_PI * j / 360.0);
    z.at(i) = log(j);
  }

  vector<double> x_scatter = {-3.0, -1.5, 1.5,  3.0,  -3.0, -1.5, 1.5,
                              3.0,  -3.0, -1.5, 1.5,  3.0,  -3.0, -1.5,
                              1.5,  3.0,  -3.0, -1.5, 1.5,  3.0};
  vector<double> y_scatter = {-2.0, -2.0, -2.0, -2.0, -1.0, -1.0, -1.0,
                              -1.0, 0.0,  0.0,  0.0,  0.0,  1.0,  1.0,
                              1.0,  1.0,  2.0,  2.0,  2.0,  2.0};
  vector<double> z_scatter = {3.0, 0.1, 0.1, 3.0, 0.1, 0.2, 0.2,
                              0.1, 0.1, 3.0, 3.0, 0.1, 0.1, 0.2,
                              0.2, 0.1, 3.0, 0.1, 0.1, 3.0};

  std::map<std::string, std::string> keywords{{"label", "Jolie courbe"}};

  SECTION("plot") {
    plt::clf();
    REQUIRE_NOTHROW(plt::plot(x, y));
    plt::pause(interval);

    plt::clf();
    REQUIRE_NOTHROW(plt::plot(y));
    plt::pause(interval);
  }

  SECTION("plot format") {
    plt::clf();
    REQUIRE_NOTHROW(plt::plot(x, w, "r--"));
    plt::pause(interval);

    plt::clf();
    REQUIRE_NOTHROW(plt::plot(y, "b+"));
    plt::pause(interval);

    plt::clf();
    REQUIRE_NOTHROW(plt::plot(x, y, "g+", x, w, "b+", x, z, "r--"));
    plt::pause(interval);
  }

  SECTION("named plot") {
    plt::clf();
    REQUIRE_NOTHROW(plt::named_plot("f(x) = z", z));
    plt::pause(interval);

    plt::clf();
    REQUIRE_NOTHROW(plt::named_plot("f(x) = y", x, y));
    plt::pause(interval);
  }

  SECTION("named plot format") {
    plt::clf();
    REQUIRE_NOTHROW(plt::named_plot("f(x) = z", z, "b+"));
    plt::pause(interval);

    plt::clf();
    REQUIRE_NOTHROW(plt::named_plot("f(x) = y", x, y, "r--"));
    plt::pause(interval);
  }

  SECTION("semilog") {
    plt::clf();
    REQUIRE_NOTHROW(plt::semilogx(x, y));
    plt::pause(interval);

    plt::clf();
    REQUIRE_NOTHROW(plt::semilogy(x, y));
    plt::pause(interval);
  }

  SECTION("semilog formats") {
    plt::clf();
    REQUIRE_NOTHROW(plt::semilogx(x, y, "r--"));
    plt::pause(interval);

    plt::clf();
    REQUIRE_NOTHROW(plt::semilogy(x, y, "b+"));
    plt::pause(interval);
  }

  SECTION("named semilog") {
    plt::clf();
    REQUIRE_NOTHROW(plt::named_semilogx("Semi log X", x, y));
    plt::pause(interval);

    plt::clf();
    REQUIRE_NOTHROW(plt::named_semilogy("Semi log Y", x, y));
    plt::pause(interval);
  }

  SECTION("named semilog format") {
    plt::clf();
    REQUIRE_NOTHROW(plt::named_semilogx("Semi log X", x, y, "r--"));
    plt::pause(interval);

    plt::clf();
    REQUIRE_NOTHROW(plt::named_semilogy("Semi log Y", x, y, "b+"));
    plt::pause(interval);
  }

  SECTION("loglog") {
    plt::clf();
    REQUIRE_NOTHROW(plt::loglog(x, y));
    plt::pause(interval);

    plt::clf();
    REQUIRE_NOTHROW(plt::named_loglog("Log log", x, y));
    plt::pause(interval);
  }

  SECTION("loglog format") {
    plt::clf();
    REQUIRE_NOTHROW(plt::loglog(x, y, "r--"));
    plt::pause(interval);

    plt::clf();
    REQUIRE_NOTHROW(plt::named_loglog("Log log", x, y, "b+"));
    plt::pause(interval);
  }

  SECTION("fill_between") {
    std::map<std::string, std::string> keywords_others_plots{
        {"facecolor", "green"}, {"interpolate", "True"}};

    plt::clf();
    REQUIRE_NOTHROW(plt::fill_between(x, y, w, keywords_others_plots));
    plt::pause(interval);
  }

  SECTION("fill") {
    std::map<std::string, std::string> keywords_others_plots{
        {"facecolor", "blue"}};
    plt::clf();
    REQUIRE_NOTHROW(plt::fill(x, y, keywords_others_plots));
    plt::pause(interval);
  }

  SECTION("stem") {
    // x = np.linspace(0.1, 2 * np.pi, 10)
    std::vector<double> x_data = {
        0.1,        0.78702059, 1.47404118, 2.16106177, 2.84808236,
        3.53510295, 4.22212354, 4.90914413, 5.59616472, 6.28318531};
    std::vector<double> y_data = {
        0.99500417,  0.70595862,  0.09660425, -0.55658157, -0.9572342,
        -0.92356879, -0.47086008, 0.19548812, 0.77313909,  1.};
    std::map<std::string, std::string> keywords_stem{{"markerfmt", "C0o"}};

    plt::clf();
    REQUIRE_NOTHROW(plt::stem(x_data));
    plt::pause(interval);

    plt::clf();
    REQUIRE_NOTHROW(plt::stem(x_data, "-."));
    plt::pause(interval);

    plt::clf();
    REQUIRE_NOTHROW(plt::stem(x_data, y_data));
    plt::pause(interval);

    plt::clf();
    REQUIRE_NOTHROW(plt::stem(x_data, y_data, "-."));
    plt::pause(interval);

    plt::clf();
    REQUIRE_NOTHROW(plt::stem(x_data, y_data, keywords_stem));
    plt::pause(interval);
  }

  SECTION("hist") {
    std::vector<double> data = {115.73956466, 112.99148762, 108.267019,
                                125.92116686, 90.26067825,  92.82370635};
    plt::clf();
    REQUIRE_NOTHROW(plt::hist(data));
    plt::pause(interval);

    plt::clf();
    REQUIRE_NOTHROW(plt::hist(data, 50, string("r"), 0.9));
    plt::pause(interval);

    plt::clf();
    REQUIRE_NOTHROW(plt::named_hist("histogramme", data));
    plt::pause(interval);

    plt::clf();
    REQUIRE_NOTHROW(plt::named_hist("histogramme", data, 40, string("g"), 0.5));
    plt::pause(interval);
  }

  SECTION("boxplot") {
    std::vector<double> test_data = {15.1, 80.1, 40.8, 3.22, 5.7, 5.98, 55.8,
                                     56.9, 69.7, 37.5, 66.1, 13.0, 46.8, 55.4,
                                     43.7, 39.5, 71.0, 28.6, 92.1, 44.1, 10.9,
                                     53.0, 3.59, 97.4, 15.1, 56.6, 35.8, 47.1,
                                     98.0, 22.6, 43.7, 21.3, 91.6, 16.0, 6.44,
                                     58.5, 70.1, 63.6, 44.3, 29.7, 15.6, 87.6,
                                     25.8, 92.4, 56.5, 82.2, 58.7, 18.2, 85.1,
                                     71.6, 50.0, 50.0, 50.0, 50.0, 50.0, 50.0,
                                     50.0, 50.0, 50.0, 50.0, 50.0, 50.0, 50.0,
                                     50.0, 50.0, 50.0, 50.0, 50.0, 50.0, 50.0,
                                     50.0, 50.0, 50.0, 50.0, 50.0, 1.53e+02,
                                     1.04e+02, 1.95e+02, 1.87e+02, 1.99e+02,
                                     1.98e+02, 1.28e+02, 1.77e+02, 1.29e+02,
                                     1.7e+02, -65.8, -31.2, -54.0, -47.8, -49.0,
                                     -30.9, -92.1, -55.1, -71.0, -30.8};
    std::vector<std::vector<double>> test_data_2 = {
        {1.19, 14.4, 15.1, 14.2, 13.7, 16.5, 9.98, 17.2, 6.49, 16.7, 15.0, 6.39,
         17.1, 15.2, 11.6, 7.09, 4.31, 16.2, 3.27, 3.26, 8.42, 13.3, 4.32, 8.76,
         10.9, 13.1, 16.8, 0.566, 14.1, 0.667, 17.1, 18.6, 10.4, 0.879, 1.73,
         12.9, 7.28, 9.91, 18.2, 13.9, 16.8, 10.2, 3.35, 1.14, 14.1, 8.61, 17.6,
         3.72, 5.43, 2.88, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0,
         10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0,
         10.0, 10.0, 10.0, 10.0, 1.08e+02, 1.05e+02, 1.19e+02, 1.02e+02,
         1.14e+02, 1.05e+02, 1.15e+02, 1.05e+02, 1.17e+02, 1.05e+02, -82.8,
         -32.7, -63.2, -95.5, -11.2, -84.9, -30.0, -16.9, -39.8, -42.8
        },
        {10.8, 14.0, 20.8, 4.15, 14.6, 7.54, 13.5, 16.7, 11.3, 6.65, 15.7, 2.95,
         13.4, 24.6, 11.9, 23.3, 16.1, 15.1, 15.6, 15.9, 15.8, 0.719, 15.7,
         12.3, 22.2, 13.7, 11.7, 9.65, 17.9, 21.0, 18.2, 20.4, 14.4, 1.88, 6.85,
         9.71, 15.5, 23.2, 22.0, 24.1, 9.43, 12.5, 5.95, 22.5, 7.36, 24.6, 7.75,
         20.3, 4.42, 8.28, 12.0, 12.0, 12.0, 12.0, 12.0, 12.0, 12.0, 12.0, 12.0,
         12.0, 12.0, 12.0, 12.0, 12.0, 12.0, 12.0, 12.0, 12.0, 12.0, 12.0, 12.0,
         12.0, 12.0, 12.0, 12.0, 1.06e+02, 1.08e+02, 1.2e+02, 1.06e+02,
         1.12e+02, 1.15e+02, 1.1e+02, 1.21e+02, 1.13e+02, 1.07e+02, -12.3,
         -67.8, -1.9, -33.1, -5.08, -26.9, -67.6, -69.9, -77.8, -12.2}
    };
    std::vector<std::string> labels = {"d1: 20", "d2: 25"};
    std::map<std::string, std::string> keywords_boxplot{{"notch", "True"}};

    plt::clf();
    REQUIRE_NOTHROW(plt::boxplot(test_data));
    plt::pause(interval);

    plt::clf();
    REQUIRE_NOTHROW(plt::boxplot(test_data, keywords_boxplot));
    plt::pause(interval);

    plt::clf();
    REQUIRE_NOTHROW(plt::boxplot(test_data_2, labels, keywords_boxplot));
    plt::pause(interval);
  }

  SECTION("bar") {
    std::vector<int> test_data = {0, 1, 2, 3, 3, 2, 6, 8, 3, 10};

    plt::clf();
    REQUIRE_NOTHROW(plt::bar(test_data));
    plt::pause(interval);

    plt::clf();
    REQUIRE_NOTHROW(plt::bar(test_data, "blue", "--", 0.5));
    plt::pause(interval);
  }

  SECTION("scatter") {
    plt::clf();
    REQUIRE_NOTHROW(plt::scatter(x_scatter, y_scatter));
    plt::pause(interval);

    std::map<std::string, std::string> kw_1{{"marker", "P"}};
    plt::clf();
    REQUIRE_NOTHROW(plt::scatter(x_scatter, y_scatter, 1.0, kw_1));
    plt::pause(interval);

    std::map<std::string, std::string> kw_2 {{"marker", "."}, {"c", "orange"}};
    plt::clf();
    REQUIRE_NOTHROW(plt::scatter(x_scatter, y_scatter, 1.0, kw_2));
    plt::pause(interval);

    plt::clf();
    REQUIRE_NOTHROW(plt::scatter(x_scatter, y_scatter, make_tuple(
        "cmap", "twilight",
        "c", z_scatter,
        "s", 46.,
        "marker", "o"
        )));
    plt::pause(interval);
  }

  SECTION("axvline") {
    std::map<std::string, std::string> kw1{{"linewidth", "2"},
                                           {"linestyle", "--"},
                                           {"color", "r"}};
    std::map<std::string, std::string> kw2{{"linewidth", "2"},
                                           {"linestyle", "--"},
                                           {"color", "b"}};

    plt::clf();
    plt::plot(x, y);
    REQUIRE_NOTHROW(plt::axvline(1.));
    plt::pause(interval);

    plt::clf();
    plt::plot(x, y);
    REQUIRE_NOTHROW(plt::axvline(1., 0., 1., kw1));
    REQUIRE_NOTHROW(plt::axvline(1., 0.20, 0.80, kw2));
    plt::pause(interval);
  }

  SECTION("quiver") {
    vector<double> xq = {-2.0, -1., 0.0, 1.0, 2.0, -2.0, -1.0, 1.0, 2.0};
    vector<double> yq = {-2.0, -1.0, 0.0, 1.0, 2.0, 2.0, 1.0, -1.0, -2.0};
    vector<double> u = {-1.0, -1.0, 0.0, 1.0, 1.0, 2.0, 1.0, -1.0, -2.0};
    vector<double> v = {1.0, -1.0, 2.0, -1.0, 1.0, -1.0, -2.0, 2.0, 1.0};

    plt::clf();
    REQUIRE_NOTHROW(plt::quiver(xq, yq, u, v));
    plt::pause(interval);
  }

  SECTION("errorbar") {
    std::map<std::string, std::string> keywords_errorbar{{"uplims", "1"}};

    plt::clf();
    REQUIRE_NOTHROW(plt::errorbar(x, y, w));
    plt::pause(interval);

    // @FIXME
    //        plt::clf();
    //        REQUIRE_NOTHROW(plt::errorbar(x, y, w, "r--"));
    //        plt::pause(interval);

    plt::clf();
    REQUIRE_NOTHROW(plt::errorbar(x, y, w, keywords_errorbar));
    plt::pause(interval);
  }

  SECTION("limits") {
    plt::clf();
    plt::plot(x, y);
    plt::plot(x, z);
    double min_ = 10.;
    double max_ = 1000.;
    REQUIRE_NOTHROW(plt::ylim(min_, max_));
    REQUIRE_NOTHROW(plt::xlim(min_, max_));
    plt::pause(interval);

    double *__xlim = plt::xlim();
    double *__ylim = plt::ylim();

    REQUIRE(__xlim[0] == Approx(min_));
    REQUIRE(__xlim[1] == Approx(max_));

    REQUIRE(__ylim[0] == Approx(min_));
    REQUIRE(__ylim[1] == Approx(max_));
  }

  SECTION("subplot") {
    plt::clf();
    plt::plot(x, y);

    REQUIRE_NOTHROW(plt::subplot(2, 1, 1));
    plt::plot(x, w);
    REQUIRE_NOTHROW(plt::subplot(2, 1, 2));
    plt::plot(x, z);
    plt::pause(interval);

    REQUIRE_NOTHROW(plt::tight_layout());
    plt::pause(interval);

    REQUIRE_NOTHROW(plt::annotate("THE point", 2, 2));
    plt::pause(interval);

    REQUIRE_NOTHROW(plt::grid(true));
    plt::pause(interval);

    REQUIRE_NOTHROW(plt::xlabel("THE XXXXX axis"));
    REQUIRE_NOTHROW(plt::ylabel("THE YYYYY axis"));
    REQUIRE_NOTHROW(plt::text(10, 5, "THE text"));
    plt::pause(interval);

    REQUIRE_NOTHROW(plt::title("THE title"));
    REQUIRE_NOTHROW(plt::suptitle("THE sup titre"));
    REQUIRE_NOTHROW(plt::axis("tight"));
    plt::pause(interval);

    REQUIRE_NOTHROW(plt::grid(false));
    plt::pause(interval);

    REQUIRE_NOTHROW(plt::pause(2));
    REQUIRE_NOTHROW(plt::save("/tmp/toto.png"));

    std::map<std::string, double> keywords_adjusts{
        {"hspace", 0.5}, {"bottom", 0.1}, {"right", 0.8}, {"top", 0.9}};
    REQUIRE_NOTHROW(plt::subplots_adjust(keywords_adjusts));

    plt::pause(interval);
  }

  SECTION("subplot2grid") {
    plt::clf();
    REQUIRE_NOTHROW(plt::figure());
    REQUIRE_NOTHROW(plt::subplot2grid(3, 3, 0, 0, 1, 3));
    REQUIRE_NOTHROW(plt::subplot2grid(3, 3, 0, 0, 1, 3));
    REQUIRE_NOTHROW(plt::subplot2grid(3, 3, 1, 0, 1, 2));
    REQUIRE_NOTHROW(plt::subplot2grid(3, 3, 1, 2, 2, 1));
    REQUIRE_NOTHROW(plt::subplot2grid(3, 3, 2, 0));
    REQUIRE_NOTHROW(plt::subplot2grid(3, 3, 2, 1));
  }

  SECTION("ticks") {
    vector<double> ticks = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    vector<std::string> labels = {"a", "b", "c", "d", "e", "f",
                                  "g", "h", "i", "j", "k", "l"};
    std::map<std::string, std::string> keywords_ticks{{"rotation", "20"}};

    plt::clf();
    plt::plot(x, y);
    REQUIRE_NOTHROW(plt::xticks(ticks));
    REQUIRE_NOTHROW(plt::xticks(ticks, keywords_ticks));
    REQUIRE_NOTHROW(plt::xticks(ticks, labels, keywords_ticks));
    plt::pause(interval);

    plt::clf();
    plt::plot(x, y);
    REQUIRE_NOTHROW(plt::yticks(ticks));
    REQUIRE_NOTHROW(plt::yticks(ticks, keywords_ticks));
    REQUIRE_NOTHROW(plt::yticks(ticks, labels, keywords_ticks));
    plt::pause(interval);

    plt::clf();
    plt::plot(x, y);
    std::map<std::string, std::string> kw_params {{"colors", "r"},
                                                  {"grid_color", "r"},
                                                  };
    REQUIRE_NOTHROW(plt::tick_params(kw_params));
    // if manual interction is needed (e.g. mouse clicked)
    //        REQUIRE_NOTHROW(plt::ginput());
  }

  SECTION("imshow_colorbar") {
    // copy from example colorbar.cpp
    auto ncols = 500, nrows = 300;
    std::vector<float> z(ncols * nrows);
    for (int j=0; j<nrows; ++j) {
      for (int i=0; i<ncols; ++i) {
        z.at(ncols * j + i) = std::sin(std::hypot(i - ncols/2, j - nrows/2));
      }
    }
    std::map<std::string, float> keywords_imc{{"shrink", 0.8}};

    const float* zptr = &(z[0]);
    const int colors = 1;

    plt::clf();
    PyObject* mat_1 = nullptr;
    REQUIRE_NOTHROW(plt::imshow(zptr, nrows, ncols, colors, {}, &mat_1));
    REQUIRE_NOTHROW(plt::colorbar(mat_1));
    plt::pause(interval);

    plt::clf();
    PyObject* mat_2 = nullptr;
    REQUIRE_NOTHROW(plt::imshow(zptr, nrows, ncols, colors, {}, &mat_2));
    REQUIRE_NOTHROW(plt::colorbar(mat_2));
    plt::pause(interval);

    plt::clf();
    vector<double> extent_data = {50., 450., 50., 250.};
    PyObject* mat_3 = nullptr;
    REQUIRE_NOTHROW(plt::imshow(zptr, nrows, ncols, colors,
                                make_tuple("extent", extent_data),
                                &mat_2));
    REQUIRE_NOTHROW(plt::colorbar(mat_2));
    plt::pause(interval);
  }
}

TEST_CASE("display_vectors", "[matplotlibcpp][matplotlib][quiver][streamplot]")
{
  // Some data
  unsigned long nx = 5;
  unsigned long ny = 5;
  vector<double> x = {1.2, 2.4, 3.6, 4.8, 6, 1.2, 2.4, 3.6, 4.8, 6, 1.2, 2.4, 3.6, 4.8, 6, 1.2, 2.4, 3.6, 4.8, 6, 1.2, 2.4, 3.6, 4.8, 6};
  vector<double> y = {1.5, 1.5, 1.5, 1.5, 1.5, 3, 3, 3, 3, 3, 4.5, 4.5, 4.5, 4.5, 4.5, 6, 6, 6, 6, 6, 7.5, 7.5, 7.5, 7.5, 7.5};
  vector<double> u = {0.362358, -0.737394, -0.896758, 0.087499, 0.96017, 0.362358, -0.737394, -0.896758, 0.087499, 0.96017, 0.362358, -0.737394, -0.896758, 0.087499, 0.96017, 0.362358, -0.737394, -0.896758, 0.087499, 0.96017, 0.362358, -0.737394, -0.896758, 0.087499, 0.96017};
  vector<double> v = {0.997495, 0.997495, 0.997495, 0.997495, 0.997495, 0.14112, 0.14112, 0.14112, 0.14112, 0.14112, -0.97753, -0.97753, -0.97753, -0.97753, -0.97753, -0.279415, -0.279415, -0.279415, -0.279415, -0.279415, 0.938, 0.938, 0.938, 0.938, 0.938};
  vector<double> mag = {1.92094, 2.83019, 3.9, 5.02892, 6.18466, 3.2311, 3.84187, 4.68615, 5.66039, 6.7082, 4.65725, 5.1, 5.76281, 6.57951, 7.5, 6.11882, 6.4622, 6.99714, 7.68375, 8.48528, 7.59539, 7.87464, 8.31925, 8.90449, 9.60469};
  vector<double> x1d = {1.2, 2.4, 3.6, 4.8, 6};
  vector<double> y1d = {1.5, 3, 4.5, 6, 7.5};
  vector<vector<double>> u2d = {{0.362358, -0.737394, -0.896758, 0.087499, 0.96017},
                                {0.362358, -0.737394, -0.896758, 0.087499, 0.96017},
                                {0.362358, -0.737394, -0.896758, 0.087499, 0.96017},
                                {0.362358, -0.737394, -0.896758, 0.087499, 0.96017},
                                {0.362358, -0.737394, -0.896758, 0.087499, 0.96017}};
  vector<vector<double>> v2d = {{0.997495, 0.997495, 0.997495, 0.997495, 0.997495},
                                {0.14112, 0.14112, 0.14112, 0.14112, 0.14112},
                                {-0.97753, -0.97753, -0.97753, -0.97753, -0.97753},
                                {-0.279415, -0.279415, -0.279415, -0.279415, -0.279415},
                                {0.938, 0.938, 0.938, 0.938, 0.938}};
  vector<vector<double>> mag2d = {{1.92094, 2.83019, 3.9, 5.02892, 6.18466},
                                  {3.2311, 3.84187, 4.68615, 5.66039, 6.7082},
                                  {4.65725, 5.1, 5.76281, 6.57951, 7.5},
                                  {6.11882, 6.4622, 6.99714, 7.68375, 8.48528},
                                  {7.59539, 7.87464, 8.31925, 8.90449, 9.60469}};

  SECTION("quiver") {
    plt::clf();
    REQUIRE_NOTHROW(plt::quiver(x, y, u, v));
    plt::pause(interval);
  }

  SECTION("quiver_with_args") {
    plt::clf();
    REQUIRE_NOTHROW(plt::quiver(x, y, u, v, make_tuple("headwidth", 5.)));
    plt::pause(interval);
  }

  SECTION("colored_quiver") {
    plt::clf();
    REQUIRE_NOTHROW(plt::quiver(x, y, u, v, mag));
    plt::pause(interval);
  }

  SECTION("colored_quiver_with_args") {
    plt::clf();
    REQUIRE_NOTHROW(plt::quiver(x, y, u, v, make_tuple("headwidth", 5.)));
    plt::pause(interval);
  }

  SECTION("streamplot") {
    plt::clf();
    REQUIRE_NOTHROW(plt::streamplot(x1d, y1d, u2d, v2d));
    plt::pause(interval);
  }

  SECTION("colored_streamplot") {
    plt::clf();
    REQUIRE_NOTHROW(plt::streamplot(x1d, y1d, u2d, v2d,  make_tuple("color", mag2d)));
    plt::pause(interval);
  }

  SECTION("colored_streamplot_with_args") {
    vector<vector<double>> lw = mag2d;
    auto it_max = max_element(mag.begin(), mag.end());
    auto max_mag = *it_max;
    for(auto&& ey: lw) {
      for(auto&& ex: ey) {
        ex = (5 * ex / max_mag);
      }
    }
    plt::clf();
    REQUIRE_NOTHROW(plt::streamplot(x1d, y1d, u2d, v2d,  make_tuple("color", mag2d, "linewidth", lw)));
    plt::pause(interval);
  }
}

TEST_CASE("matplotlib3d",
          "[matplotlibcpp][matplotlibcpp3d][matplotlib][matplotlib3d]") {

  std::vector<std::vector<double>> x, y, z;
  auto i = -5;
  auto cpt_end = 40;
  for (auto cpt_i = 0; cpt_i < cpt_end; ++cpt_i) {
    std::vector<double> x_row, y_row, z_row;
    auto j = -5;
    for (auto cpt_j = 0; cpt_j < cpt_end; ++cpt_j) {
      x_row.push_back(i);
      y_row.push_back(j);
      z_row.push_back(std::sin(std::hypot(i, j)));
      j += 0.25;
    }
    i += 0.25;
    x.push_back(x_row);
    y.push_back(y_row);
    z.push_back(z_row);
  }

  std::vector<double> xx, yy, zz;
  i = -5;
  for (auto cpt = 0; cpt < 40; ++cpt) {
    xx.push_back(i);
    yy.push_back(i);
    zz.push_back(std::sin(std::hypot(i, i)));
    i += 0.25;
  }

  std::vector<double> xxx, yyy, zzz;
  i = -5;
  for (auto cpt = 0; cpt < 40; ++cpt) {
    xxx.push_back(i);
    yyy.push_back(std::sin(std::hypot(i, i)));
    i += 0.25;
  }
  zzz.push_back(2.);

  SECTION("plot_surface") {
    plt::clf();
    REQUIRE_NOTHROW(plt::plot_surface(x, y, z));
    plt::pause(interval);
  }

  SECTION("plot_surface_labels") {
    plt::clf();
    plt::plot_surface(x, y, z);
    REQUIRE_NOTHROW(plt::xlabel("abscissa"));
    REQUIRE_NOTHROW(plt::ylabel("ordinate"));
    REQUIRE_NOTHROW(plt::set_zlabel("applicate"));
    plt::pause(interval);
  }

  SECTION("cla") {
    plt::clf();
    plt::plot_surface(x, y, z);
    REQUIRE_NOTHROW(plt::cla());
    plt::plot_surface(x, y, z);
    plt::pause(interval);
  }
}

TEST_CASE("matplotlib_ending", "[matplotlibcpp][matplotlib]") {
  // Some data
  unsigned long n = 5000;
  std::vector<double> x(n), y(n);
  for (unsigned long i = 0; i < n; ++i) {
    x.at(i) = i * i;
    y.at(i) = sin(2 * M_PI * static_cast<double>(i) / 360.0);
  }

//  REQUIRE_NOTHROW(plt::backend("Qt5Agg"));
  plt::ion();
  plt::named_plot("toto", x, y);

  REQUIRE_NOTHROW(plt::legend());
  REQUIRE_NOTHROW(plt::draw());
  long fig_num = 111;
  REQUIRE_NOTHROW(plt::figure(fig_num));
  CHECK(plt::fignum_exists(fig_num));
  REQUIRE_NOTHROW(plt::xkcd());
  plt::clf();
  plt::named_plot("Courbe", x, y);
  plt::show(true);
  REQUIRE_NOTHROW(plt::close(fig_num));
}

TEST_CASE("matplotlib_figure", "[matplotlibcpp]") {
  plt::ion();

  SECTION("figure_size") {
    REQUIRE_NOTHROW(plt::figure_size(100, 100));
    plt::show(true);
    REQUIRE_NOTHROW(plt::close());
  }

  SECTION("figure_template") {
    vector<double> fsize {6., 4.};
    REQUIRE_NOTHROW(plt::figure(111, make_tuple(
        "dpi", 50,
        "edgecolor", "green",
        "figsize", fsize
    )));
  }
}
#endif // CATCH_UNIT_TESTS
