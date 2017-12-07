examples: minimal basic modern animation nonblock xkcd

minimal: examples/minimal.cpp matplotlibcpp.h
	cd examples && g++ -DWITHOUT_NUMPY minimal.cpp -I/usr/include/python2.7 -lpython2.7 -o minimal -std=c++11

basic: examples/basic.cpp matplotlibcpp.h
	cd examples && g++ basic.cpp -I/usr/include/python2.7 -lpython2.7 -o basic

modern: examples/modern.cpp matplotlibcpp.h
	cd examples && g++ modern.cpp -I/usr/include/python2.7 -lpython2.7 -o modern -std=c++11

animation: examples/animation.cpp matplotlibcpp.h
	cd examples && g++ animation.cpp -I/usr/include/python2.7 -lpython2.7 -o animation -std=c++11

nonblock: examples/nonblock.cpp matplotlibcpp.h
	cd examples && g++ nonblock.cpp -I/usr/include/python2.7 -lpython2.7 -o nonblock -std=c++11

xkcd: examples/xkcd.cpp matplotlibcpp.h
	cd examples && g++ xkcd.cpp -I/usr/include/python2.7 -lpython2.7 -o xkcd -std=c++11

clean:
	rm -f examples/{minimal,basic,modern,animation,nonblock,xkcd}
