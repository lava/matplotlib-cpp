examples: minimal basic modern animation nonblock xkcd

INCLUDES := `python-config --includes`
LIBS := `python-config --libs`

minimal: examples/minimal.cpp matplotlibcpp.h
	cd examples && g++ -DWITHOUT_NUMPY minimal.cpp $(INCLUDES) $(LIBS) -o minimal -std=c++11

basic: examples/basic.cpp matplotlibcpp.h
	cd examples && g++ basic.cpp $(INCLUDES) $(LIBS) -o basic

modern: examples/modern.cpp matplotlibcpp.h
	cd examples && g++ modern.cpp $(INCLUDES) $(LIBS) -o modern -std=c++11

animation: examples/animation.cpp matplotlibcpp.h
	cd examples && g++ animation.cpp $(INCLUDES) $(LIBS) -o animation -std=c++11

nonblock: examples/nonblock.cpp matplotlibcpp.h
	cd examples && g++ nonblock.cpp $(INCLUDES) $(LIBS) -o nonblock -std=c++11

xkcd: examples/xkcd.cpp matplotlibcpp.h
	cd examples && g++ xkcd.cpp $(INCLUDES) $(LIBS) -o xkcd -std=c++11

clean:
	rm -f examples/{minimal,basic,modern,animation,nonblock,xkcd}
