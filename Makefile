examples: minimal basic modern animation nonblock xkcd quiver bar surface fill_inbetween fill update

minimal: examples/minimal.cpp matplotlibcpp.h
	cd examples && g++ -DWITHOUT_NUMPY minimal.cpp -I/usr/include/python2.7 -lpython2.7 -o minimal -std=c++11

basic: examples/basic.cpp matplotlibcpp.h
	cd examples && g++ basic.cpp -I/usr/include/python2.7 -lpython2.7 -o basic -std=c++11

modern: examples/modern.cpp matplotlibcpp.h
	cd examples && g++ modern.cpp -I/usr/include/python2.7 -lpython2.7 -o modern -std=c++11

animation: examples/animation.cpp matplotlibcpp.h
	cd examples && g++ animation.cpp -I/usr/include/python2.7 -lpython2.7 -o animation -std=c++11

nonblock: examples/nonblock.cpp matplotlibcpp.h
	cd examples && g++ nonblock.cpp -I/usr/include/python2.7 -lpython2.7 -o nonblock -std=c++11

quiver: examples/quiver.cpp matplotlibcpp.h
	cd examples && g++ quiver.cpp -I/usr/include/python2.7 -lpython2.7 -o quiver -std=c++11

xkcd: examples/xkcd.cpp matplotlibcpp.h
	cd examples && g++ xkcd.cpp -I/usr/include/python2.7 -lpython2.7 -o xkcd -std=c++11

bar: examples/bar.cpp matplotlibcpp.h
	cd examples && g++ bar.cpp -I/usr/include/python2.7 -lpython2.7 -o bar -std=c++11

surface: examples/surface.cpp matplotlibcpp.h
	cd examples && g++ surface.cpp -I/usr/include/python2.7 -lpython2.7 -o surface -std=c++11

fill_inbetween: examples/fill_inbetween.cpp matplotlibcpp.h
	cd examples && g++ fill_inbetween.cpp -I/usr/include/python2.7 -lpython2.7 -o fill_inbetween -std=c++11

fill: examples/fill.cpp matplotlibcpp.h
	cd examples && g++ fill.cpp -I/usr/include/python2.7 -lpython2.7 -o fill -std=c++11
	
update: examples/update.cpp matplotlibcpp.h
	cd examples && g++ update.cpp -I/usr/include/python2.7 -lpython2.7 -o update -std=c++11

clean:
	rm -f examples/{minimal,basic,modern,animation,nonblock,xkcd,quiver,bar,surface,fill_inbetween,fill,update}
