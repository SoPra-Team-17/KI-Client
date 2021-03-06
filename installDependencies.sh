#!/usr/bin/env bash

# Setup
# Exit the script if any command fails
set -e

# Dependencies from LibClient
cd extern/LibClient
./installDependencies.sh

# CLI11
cd /tmp
git clone --depth 1 -b v1.9.0 https://github.com/CLIUtils/CLI11.git
cd CLI11
mkdir build && cd build
cmake -DCLI11_BUILD_TESTS=false -DCLI11_BUILD_DOCS=false -DCLI11_BUILD_EXAMPLES=false ..
make -j$(nproc)
sudo make install

# spdlog
cd /tmp
git clone --depth 1 -b v1.6.1 https://github.com/gabime/spdlog.git
cd spdlog
mkdir build && cd build
cmake -DSPDLOG_BUILD_TESTS=false -DSPDLOG_BUILD_EXAMPLE=false ..
make -j$(nproc)
sudo make install


sudo ldconfig