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
  std::map<std::string, std::string> keywords{{"label", "Jolie courbe"}};

  // To be sure that plt::clf() call doesn't crash.
  plt::ion();

  SECTION("plot") {
    plt::clf();
    REQUIRE_NOTHROW(plt::plot(x, y));
    plt::show(true);

    plt::clf();
    REQUIRE_NOTHROW(plt::plot(y));
    plt::show(true);
  }

  SECTION("plot format") {
    plt::clf();
    REQUIRE_NOTHROW(plt::plot(x, w, "r--"));
    plt::show(true);

    plt::clf();
    REQUIRE_NOTHROW(plt::plot(y, "b+"));
    plt::show(true);

    plt::clf();
    REQUIRE_NOTHROW(plt::plot(x, y, "g+", x, w, "b+", x, z, "r--"));
    plt::show(true);
  }

  SECTION("named plot") {
    plt::clf();
    REQUIRE_NOTHROW(plt::named_plot("f(x) = z", z));
    plt::show(true);

    plt::clf();
    REQUIRE_NOTHROW(plt::named_plot("f(x) = y", x, y));
    plt::show(true);
  }

  SECTION("named plot format") {
    plt::clf();
    REQUIRE_NOTHROW(plt::named_plot("f(x) = z", z, "b+"));
    plt::show(true);

    plt::clf();
    REQUIRE_NOTHROW(plt::named_plot("f(x) = y", x, y, "r--"));
    plt::show(true);
  }

  SECTION("semilog") {
    plt::clf();
    REQUIRE_NOTHROW(plt::semilogx(x, y));
    plt::show(true);

    plt::clf();
    REQUIRE_NOTHROW(plt::semilogy(x, y));
    plt::show(true);
  }

  SECTION("semilog formats") {
    plt::clf();
    REQUIRE_NOTHROW(plt::semilogx(x, y, "r--"));
    plt::show(true);

    plt::clf();
    REQUIRE_NOTHROW(plt::semilogy(x, y, "b+"));
    plt::show(true);
  }

  SECTION("named semilog") {
    plt::clf();
    REQUIRE_NOTHROW(plt::named_semilogx("Semi log X", x, y));
    plt::show(true);

    plt::clf();
    REQUIRE_NOTHROW(plt::named_semilogy("Semi log Y", x, y));
    plt::show(true);
  }

  SECTION("named semilog format") {
    plt::clf();
    REQUIRE_NOTHROW(plt::named_semilogx("Semi log X", x, y, "r--"));
    plt::show(true);

    plt::clf();
    REQUIRE_NOTHROW(plt::named_semilogy("Semi log Y", x, y, "b+"));
    plt::show(true);
  }

  SECTION("loglog") {
    plt::clf();
    REQUIRE_NOTHROW(plt::loglog(x, y));
    plt::show(true);

    plt::clf();
    REQUIRE_NOTHROW(plt::named_loglog("Log log", x, y));
    plt::show(true);
  }

  SECTION("loglog format") {
    plt::clf();
    REQUIRE_NOTHROW(plt::loglog(x, y, "r--"));
    plt::show(true);

    plt::clf();
    REQUIRE_NOTHROW(plt::named_loglog("Log log", x, y, "b+"));
    plt::show(true);
  }

  SECTION("fill_between") {
    std::map<std::string, std::string> keywords_others_plots{
        {"facecolor", "green"}, {"interpolate", "True"}};

    plt::clf();
    REQUIRE_NOTHROW(plt::fill_between(x, y, w, keywords_others_plots));
    plt::show(true);
  }

  SECTION("fill") {
    std::map<std::string, std::string> keywords_others_plots{
        {"facecolor", "blue"}};
    plt::clf();
    REQUIRE_NOTHROW(plt::fill(x, y, keywords_others_plots));
    plt::show(true);
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
    plt::show(true);

    plt::clf();
    REQUIRE_NOTHROW(plt::stem(x_data, "-."));
    plt::show(true);

    plt::clf();
    REQUIRE_NOTHROW(plt::stem(x_data, y_data));
    plt::show(true);

    plt::clf();
    REQUIRE_NOTHROW(plt::stem(x_data, y_data, "-."));
    plt::show(true);

    plt::clf();
    REQUIRE_NOTHROW(plt::stem(x_data, y_data, keywords_stem));
    plt::show(true);
  }

  SECTION("hist") {
    std::vector<double> data = {115.73956466, 112.99148762, 108.267019,
                                125.92116686, 90.26067825,  92.82370635};
    plt::clf();
    REQUIRE_NOTHROW(plt::hist(data));
    plt::show(true);

    plt::clf();
    REQUIRE_NOTHROW(plt::hist(data, 50, string("r"), 0.9));
    plt::show(true);

    plt::clf();
    REQUIRE_NOTHROW(plt::named_hist("histogramme", data));
    plt::show(true);

    plt::clf();
    REQUIRE_NOTHROW(plt::named_hist("histogramme", data, 40, string("g"), 0.5));
    plt::show(true);
  }


  SECTION("bar") {
    std::vector<int> test_data = {0, 1, 2, 3, 3, 2, 6, 8, 3, 10};

    plt::clf();
    REQUIRE_NOTHROW(plt::bar(test_data));
    plt::show(true);

    plt::clf();
    REQUIRE_NOTHROW(plt::bar(test_data, "blue", "--", 0.5));
    plt::show(true);
  }

  SECTION("scatter") {
    vector<double> x_scatter = {-3.0, -1.5, 1.5,  3.0,  -3.0, -1.5, 1.5,
                                3.0,  -3.0, -1.5, 1.5,  3.0,  -3.0, -1.5,
                                1.5,  3.0,  -3.0, -1.5, 1.5,  3.0};
    vector<double> y_scatter = {-2.0, -2.0, -2.0, -2.0, -1.0, -1.0, -1.0,
                                -1.0, 0.0,  0.0,  0.0,  0.0,  1.0,  1.0,
                                1.0,  1.0,  2.0,  2.0,  2.0,  2.0};
    vector<double> z_scatter = {3.0, 0.1, 0.1, 3.0, 0.1, 0.2, 0.2,
                                0.1, 0.1, 3.0, 3.0, 0.1, 0.1, 0.2,
                                0.2, 0.1, 3.0, 0.1, 0.1, 3.0};

    plt::clf();
    REQUIRE_NOTHROW(plt::scatter(x_scatter, y_scatter));
    plt::show(true);

    std::unordered_map<std::string, std::string> kw_1{{"marker", "P"}};
    plt::clf();
    REQUIRE_NOTHROW(plt::scatter(x_scatter, y_scatter, 1.0, kw_1));
    plt::show(true);

    std::unordered_map<std::string, std::string> kw_2 {{"marker", "."}, {"c", "orange"}};
    plt::clf();
    REQUIRE_NOTHROW(plt::scatter(x_scatter, y_scatter, 1.0, kw_2));
    plt::show(true);

    // @FIXME
    //        plt::clf();
    //        REQUIRE_NOTHROW(plt::scatter(x_scatter, y_scatter, make_tuple(
    //                "cmap", "twilight",
    //                "c", z_scatter,
    //                "s", 46.,
    //                "marker", "o"
    //        )));
    //        plt::show(true);
  }

  SECTION("quiver") {
    vector<double> xq = {-2.0, -1., 0.0, 1.0, 2.0, -2.0, -1.0, 1.0, 2.0};
    vector<double> yq = {-2.0, -1.0, 0.0, 1.0, 2.0, 2.0, 1.0, -1.0, -2.0};
    vector<double> u = {-1.0, -1.0, 0.0, 1.0, 1.0, 2.0, 1.0, -1.0, -2.0};
    vector<double> v = {1.0, -1.0, 2.0, -1.0, 1.0, -1.0, -2.0, 2.0, 1.0};

    plt::clf();
    REQUIRE_NOTHROW(plt::quiver(xq, yq, u, v));
    plt::show(true);
  }

  SECTION("errorbar") {
    std::map<std::string, std::string> keywords_errorbar{{"uplims", "1"}};

    plt::clf();
    REQUIRE_NOTHROW(plt::errorbar(x, y, w));
    plt::show(true);

    // @FIXME
    //        plt::clf();
    //        REQUIRE_NOTHROW(plt::errorbar(x, y, w, "r--"));
    //        plt::show(true);

    plt::clf();
    REQUIRE_NOTHROW(plt::errorbar(x, y, w, keywords_errorbar));
    plt::show(true);
  }

  SECTION("limits") {
    plt::clf();
    plt::plot(x, y);
    plt::plot(x, z);
    double min_ = 10.;
    double max_ = 1000.;
    REQUIRE_NOTHROW(plt::ylim(min_, max_));
    REQUIRE_NOTHROW(plt::xlim(min_, max_));
    plt::show(true);

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
    plt::show(true);

    REQUIRE_NOTHROW(plt::tight_layout());
    plt::show(true);

    REQUIRE_NOTHROW(plt::annotate("THE point", 2, 2));
    plt::show(true);

    REQUIRE_NOTHROW(plt::grid(true));
    plt::show(true);

    REQUIRE_NOTHROW(plt::xlabel("THE XXXXX axis"));
    REQUIRE_NOTHROW(plt::ylabel("THE YYYYY axis"));
    REQUIRE_NOTHROW(plt::text(10, 5, "THE text"));
    plt::show(true);

    REQUIRE_NOTHROW(plt::title("THE title"));
    REQUIRE_NOTHROW(plt::suptitle("THE sup titre"));
    REQUIRE_NOTHROW(plt::axis("tight"));
    plt::show(true);

    REQUIRE_NOTHROW(plt::grid(false));
    plt::show(true);

    REQUIRE_NOTHROW(plt::pause(2));
    REQUIRE_NOTHROW(plt::save("/tmp/toto.png"));

    std::map<std::string, double> keywords_adjusts{
        {"hspace", 0.5}, {"bottom", 0.1}, {"right", 0.8}, {"top", 0.9}};
    REQUIRE_NOTHROW(plt::subplots_adjust(keywords_adjusts));

    plt::show(true);
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
    plt::show(true);

    plt::clf();
    plt::plot(x, y);
    REQUIRE_NOTHROW(plt::yticks(ticks));
    REQUIRE_NOTHROW(plt::yticks(ticks, keywords_ticks));
    REQUIRE_NOTHROW(plt::yticks(ticks, labels, keywords_ticks));
    plt::show(true);

    plt::clf();
    plt::plot(x, y);
    std::map<std::string, std::string> kw_params {{"colors", "r"},
                                                  {"grid_color", "r"},
                                                  };
    REQUIRE_NOTHROW(plt::tick_params(kw_params));
    // if manual interction is needed (e.g. mouse clicked)
    //        REQUIRE_NOTHROW(plt::ginput());
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
    xx.push_back(i);
    yy.push_back(std::sin(std::hypot(i, i)));
    i += 0.25;
  }
  zz.push_back(2.);

  // To be sure that plt::clf() call doesn't crash.
  plt::ion();

  SECTION("plot_surface") {
    plt::clf();
    REQUIRE_NOTHROW(plt::plot_surface(x, y, z));
    plt::show(true);
  }

  SECTION("plot_surface_2") {
    plt::clf();
    REQUIRE_NOTHROW(plt::plot_surface(x, y, z));
    plt::clf();
    plt::plot_surface(x, y, z);
    plt::show(true);
  }

  SECTION("cla") {
    plt::clf();
    plt::plot_surface(x, y, z);
    REQUIRE_NOTHROW(plt::cla());
    plt::plot_surface(x, y, z);
    plt::show(true);
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

  REQUIRE_NOTHROW(plt::backend("Qt5Agg"));
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
  REQUIRE_NOTHROW(plt::close());
}

TEST_CASE("matplotlib_figure", "[matplotlibcpp]") {
  plt::ion();
  REQUIRE_NOTHROW(plt::figure_size(100, 100));
  plt::show(true);
}
#endif // CATCH_UNIT_TESTS
