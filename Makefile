# Use C++11, dont warn on long-to-float conversion
CXXFLAGS += -std=c++11 -Wno-conversion

# Default to using system's default version of python
PYTHON_BIN     ?= python3
PYTHON_CONFIG  := $(PYTHON_BIN)-config
PYTHON_INCLUDE ?= $(shell $(PYTHON_CONFIG) --includes)
EXTRA_FLAGS    := $(PYTHON_INCLUDE)
# NOTE: Since python3.8, the correct invocation is `python3-config --libs --embed`.
# So of course the proper way to get python libs for embedding now is to
# invoke that, check if it crashes, and fall back to just `--libs` if it does.
LDFLAGS        += $(shell if $(PYTHON_CONFIG) --ldflags --embed >/dev/null; then $(PYTHON_CONFIG) --ldflags --embed; else $(PYTHON_CONFIG) --ldflags; fi)

# Either finds numpy or set -DWITHOUT_NUMPY
EXTRA_FLAGS     += $(shell $(PYTHON_BIN) $(CURDIR)/numpy_flags.py)
WITHOUT_NUMPY   := $(findstring $(EXTRA_FLAGS), WITHOUT_NUMPY)

# Examples requiring numpy support to compile
EXAMPLES_NUMPY  := surface colorbar contour spy
EXAMPLES        := minimal basic modern animation nonblock xkcd quiver bar \
	           fill_inbetween fill update subplot2grid lines3d \
                   $(if $(WITHOUT_NUMPY),,$(EXAMPLES_NUMPY))

# Prefix every example with 'examples/build/'
EXAMPLE_TARGETS := $(patsubst %,examples/build/%,$(EXAMPLES))

.PHONY: examples

examples: $(EXAMPLE_TARGETS)

docs:
	doxygen
	moxygen doc/xml --noindex -o doc/api.md

# Assume every *.cpp file is a separate example
$(EXAMPLE_TARGETS): examples/build/%: examples/%.cpp matplotlibcpp.h
	mkdir -p examples/build
	$(CXX) -o $@ $< $(EXTRA_FLAGS) $(CXXFLAGS) $(LDFLAGS)

clean:
	rm -f ${EXAMPLE_TARGETS}
