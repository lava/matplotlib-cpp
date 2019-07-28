
# Use C++11
CXXFLAGS += -std=c++11

# Default to using system's default version of python
PYTHON_BIN     ?= python
PYTHON_CONFIG  := $(PYTHON_BIN)-config
PYTHON_INCLUDE ?= $(shell $(PYTHON_CONFIG) --includes)
CXXFLAGS       += $(PYTHON_INCLUDE)
LDFLAGS        += $(shell $(PYTHON_CONFIG) --libs)

# Either finds numpy or set -DWITHOUT_NUMPY
CURRENT_DIR := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
CXXFLAGS    += $(shell $(PYTHON_BIN) $(CURRENT_DIR)/numpy_flags.py)

# Assume every *.cpp file is a separate example
SOURCES     ?= $(wildcard examples/*.cpp)
EXECUTABLES := $(foreach exec,$(basename $(SOURCES)),$(exec))

.PHONY: examples

examples: $(EXECUTABLES)

$(EXECUTABLES): %: %.cpp
	$(CXX) -o $@ $< $(CXXFLAGS) $(LDFLAGS)

clean:
	rm -f ${EXECUTABLES}
