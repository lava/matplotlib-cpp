examples: minimal basic modern

minimal: examples/minimal.cpp matplotlibcpp.h
	cd examples && g++ minimal.cpp -lpython2.7 -o minimal -std=c++11

basic: examples/basic.cpp matplotlibcpp.h
	cd examples && g++ basic.cpp -lpython2.7 -o basic

modern: examples/modern.cpp matplotlibcpp.h
	cd examples && g++ modern.cpp -lpython2.7 -o modern -std=c++11

