# KI-Client
This repository contains the AI-Client component of team 17 for the 
*No Time To Spy* game of the Softwaregrundprojekt 2019/2020 at 
the university of Ulm.  

## Installation 
The AI-Client can be installed manually and through a docker container. 

### Manual Installation
#### Prerequisites 
 * C++17 compatible Compiler (e.g. GCC-8)
 * CMake (at least version 3.10)
 * GNU-Make
 * `./installDependencies.sh`

#### Compiling the application
Create a directory for the build and change into this. The name of this 
directory can be arbitrarily chosen, in this example it will be called *build*. 
```
mkdir build && cd build
```
Generate a makefile using cmake. This step shouldn't produce any errors if 
all prerequisites are installed. 
```
cmake ..
```
Compile the application using make.
```
make
```
### Running the application
Test the installation by executing the AI-Client.
```
./src/ki017 -h
```
### Installing the API
The AI-Client provides an interface to the Choice and Operation generation methods via a library (SopraAI).
To install the library execute the following command after compiling the application:
```
sudo make install
```
The library can be linked with `-lSopraAI`.

### Docker
### Building the docker container
Build the docker container `soprateam17/ki-client` with
```
docker build -t soprateam17/ki-client .
```
or
```
make build
```
or pull the prebuilt container for the `develop` branch from dockerhub:
```
docker pull soprateam17/ki-client
```

### Running the container
Test the docker container by executing it.
```
docker run --rm --net=host soprateam17/ki-client -h
```
or
```
make run
```
To set flags like the server address (e.g. localhost) use:
```
docker run --rm --net=host soprateam17/ki-client -a 127.0.0.1
```
or
```
make run AIFLAGS="-a 127.0.0.1"
```

## Usage
Flags for the AI-Client startup:
* `-h` / `--help` prints some help information and exits
* `-a <ip address>` / `--address <ip address>` configuration of the server IP address to be used to
* `-p <int>` / `--port <int>` configuration of the server port to be used (defaults to 7007)
* `-n <string>` / `--name <string>` configuration of AI-Client name (defaults to ki017)
* `-d <int>` / `--difficulty <int>` configuration of the difficulty of the AI-Client (defaults to 0 max difficulty)
* `-v <int>` / `--verbosity <int>` configuration of the logging verbosity (defaults to max verbosity)
* `--x <key> <value>` can be used to give the server additional key-value pairs
    * `maxReconnect <int>` max number of reconnect attempts when connection to server is lost (defaults to 5)
    * `nodelay <nothing>` set this flag if AI should not delay its moves

## For Developers
The AI-Client is implemented in C++. You can use e.g. CLION as development environment.  
The following libraries where used. The respective licenses are listed below:
* LibCommon: version see submodule [GPLv3]
* LibClient: version see submodule [GPLv3]
* CLI11: version version 1.9.0 [[license](https://github.com/CLIUtils/CLI11/blob/master/LICENSE)]
* spdlog: version 1.6.1 [[MIT license](https://github.com/gabime/spdlog/blob/v1.x/LICENSE)]
