FROM debian:10 AS builder
RUN apt-get update \
 && apt-get install --yes --no-install-recommends \
    g++ \
    libpython3-dev \
    make \
    python3 \
    python3-dev \
    python3-numpy

ADD Makefile matplotlibcpp.h numpy_flags.py /opt/
ADD examples/*.cpp /opt/examples/
RUN cd /opt \
 && make PYTHON_BIN=python3 \
 && ls examples/build

FROM debian:10
RUN apt-get update \
 && apt-get install --yes --no-install-recommends \
    libpython3-dev \
    python3-matplotlib \
    python3-numpy

COPY --from=builder /opt/examples/build /opt/
RUN cd /opt \
 && ls \
 && ./basic
