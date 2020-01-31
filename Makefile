# Use C++11
MATPLOTLIBCPP_CXXFLAGS = -std=c++11

# Default to using system's default version of python
PYTHON_BIN              := python
PYTHON_CONFIG           := $(PYTHON_BIN)-config
PYTHON_INCLUDE          ?= $(shell $(PYTHON_CONFIG) --includes)
MATPLOTLIBCPP_CXXFLAGS  += $(PYTHON_INCLUDE)
MATPLOTLIBCPP_LDFLAGS   += $(shell $(PYTHON_CONFIG) --libs)

# If these checks have false positives, please remove this check and file a github issue.
$(if $(PYTHON_INCLUDE),,$(error \
Could not auto-detect python development headers. \
Please install python-dev or similar, or manually \
specify PYTHON_INCLUDE=...))

# If we have to add 1-2 more of these checks, I'll probably eventually bite the
# bullet and switch to cmake/autoconf.
test_matplotlib != $(PYTHON_BIN) -c 'import matplotlib'
WITH_MATPLOTLIB = $(.SHELLSTATUS)
$(ifeq $(WITH

# Either finds numpy or set -DWITHOUT_NUMPY
MATPLOTLIBCPP_CXXFLAGS  += $(shell $(PYTHON_BIN) $(CURDIR)/numpy_flags.py)
WITHOUT_NUMPY           := $(findstring $(CXXFLAGS), WITHOUT_NUMPY)

# Examples requiring numpy support to compile
EXAMPLES_NUMPY  := surface
EXAMPLES        := minimal basic modern animation nonblock xkcd quiver bar fill_inbetween fill update subplot2grid \
                   $(if WITHOUT_NUMPY,,$(EXAMPLES_NUMPY))

# Prefix every example with 'examples/build/'
EXAMPLE_TARGETS := $(patsubst %,examples/build/%,$(EXAMPLES))

.PHONY: examples

examples: $(EXAMPLE_TARGETS)

# Assume every *.cpp file is a separate example
$(EXAMPLE_TARGETS): examples/build/%: examples/%.cpp
	mkdir -p examples/build
	$(CXX) -o $@ $< $(MATPLOTLIBCPP_CXXFLAGS) $(CXXFLAGS) $(LDFLAGS) $(MATPLOTLIBCPP_LDFLAGS)

clean:
	rm -f ${EXAMPLE_TARGETS}

install:
	cp matplotlibcpp.h $(DESTDIR)$(PREFIX)/include/matplotlibcpp.h
