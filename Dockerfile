FROM ubuntu:18.04

COPY src /ki/src
COPY test /ki/test
COPY extern /ki/extern
COPY CMakeLists.txt* installDependencies.sh /ki/

WORKDIR /ki

# Dependencies
RUN apt update && apt install -y sudo git build-essential cmake g++-8
ENV CXX=g++-8
RUN ./installDependencies.sh

# Build ki
RUN mkdir build
RUN cd build && cmake -DCMAKE_BUILD_TYPE=Release ..
RUN cd build && make -j$(nproc)

ENTRYPOINT /ki/build/src/ki017