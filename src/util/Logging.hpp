//
// Created by Carolin on 16.05.2020.
//

#ifndef KICLIENT_LOGGING_HPP
#define KICLIENT_LOGGING_HPP

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

class Logging {
    public:
        Logging() = delete;

        static void initLogging(unsigned int verbosity);
};


#endif //KICLIENT_LOGGING_HPP
