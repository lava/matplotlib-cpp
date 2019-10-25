# Use C++11
CXXFLAGS += -std=c++11

# Default to using system's default version of python
PYTHON_BIN     ?= python
PYTHON_CONFIG  := $(PYTHON_BIN)-config
PYTHON_INCLUDE ?= $(shell $(PYTHON_CONFIG) --includes)
CXXFLAGS       += $(PYTHON_INCLUDE)
LDFLAGS        += $(shell $(PYTHON_CONFIG) --libs)

# Either finds numpy or set -DWITHOUT_NUMPY
CXXFLAGS        += $(shell $(PYTHON_BIN) $(CURDIR)/numpy_flags.py)
WITHOUT_NUMPY   := $(findstring $(CXXFLAGS), WITHOUT_NUMPY)

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
	$(CXX) -o $@ $< $(CXXFLAGS) $(LDFLAGS)

clean:
	rm -f ${EXAMPLE_TARGETS}
