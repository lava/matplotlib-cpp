# GNU Makefile for matplotlib-cpp examples

PYVER = 3.5m
INCDIR = /usr/include/python$(PYVER)
LIBDIR = /usr/lib/python$(PYVER)
CXXFLAGS = -O2 -Wall -std=c++11 -I$(INCDIR) -L$(LIBDIR) -lpython$(PYVER)

examples = minimal basic modern animation nonblock xkcd quiver bar surface fill_inbetween fill update imshow colorbar

all: $(examples)
.PHONY: all clean

minimal: minimal.cpp ../matplotlibcpp.h
	g++ -DWITHOUT_NUMPY $< -o $@ $(CXXFLAGS)

basic: basic.cpp ../matplotlibcpp.h
	g++ $< -o $@ $(CXXFLAGS)

modern: modern.cpp ../matplotlibcpp.h
	g++ $< -o $@ $(CXXFLAGS)

animation: animation.cpp ../matplotlibcpp.h
	g++ $< -o $@ $(CXXFLAGS)

nonblock: nonblock.cpp ../matplotlibcpp.h
	g++ $< -o $@ $(CXXFLAGS)

quiver: quiver.cpp ../matplotlibcpp.h
	g++ $< -o $@ $(CXXFLAGS)

xkcd: xkcd.cpp ../matplotlibcpp.h
	g++ $< -o $@ $(CXXFLAGS)

bar: bar.cpp ../matplotlibcpp.h
	g++ $< -o $@ $(CXXFLAGS)

surface: surface.cpp ../matplotlibcpp.h
	g++ $< -o $@ $(CXXFLAGS)

colorbar: colorbar.cpp ../matplotlibcpp.h
	g++ $< -o $@ $(CXXFLAGS)

fill_inbetween: fill_inbetween.cpp ../matplotlibcpp.h
	g++ $< -o $@ $(CXXFLAGS)

fill: fill.cpp ../matplotlibcpp.h
	g++ $< -o $@ $(CXXFLAGS)

update: update.cpp ../matplotlibcpp.h
	g++ $< -o $@ $(CXXFLAGS)

imshow: imshow.cpp ../matplotlibcpp.h
	g++ $< -o $@ $(CXXFLAGS)

clean:
	rm -f $(examples)
