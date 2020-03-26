# Tests with Catch2

Here some tests using [Catch2](https://github.com/catchorg/Catch2).

Only tested on Ubuntu and CentOS 7, with `gcc` 6.2, 6.3 and 8.3.

**warning** I force `Qt5Agg` backend. But it should work with other backend, except 'Agg'.

For running the tests :

    cd <matplotlibcpp_home>/contrib/tests_catch2
    cmake -S . -B "./build"
    cd build
    make