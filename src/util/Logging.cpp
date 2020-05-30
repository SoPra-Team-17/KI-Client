//
// Created by Carolin on 16.05.2020.
//


#include "Logging.hpp"

void Logging::initLogging(unsigned int verbosity) {
    std::vector<spdlog::sink_ptr> sinks;
    std::string logFile(30, '\0');
    struct tm buf = {};

    std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    std::strftime(&logFile[0], logFile.size(), "%m-%d_%H:%M:%S.txt", localtime_r(&now, &buf));

    // logging to console can be influenced via verbosity setting
    auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    consoleSink->set_color_mode(spdlog::color_mode::always);
    verbosity = std::abs(static_cast<int>(verbosity - spdlog::level::level_enum::n_levels));
    consoleSink->set_level(static_cast<spdlog::level::level_enum>((verbosity)));

    // logging to file always works with max logging level
    auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/" + logFile);
    fileSink->set_level(spdlog::level::level_enum::trace);

    sinks.push_back(consoleSink);
    sinks.push_back(fileSink);

    auto combined_logger = std::make_shared<spdlog::logger>("Logger", begin(sinks), end(sinks));

    // Flush for every message of level info or higher
    combined_logger->flush_on(spdlog::level::info);

    combined_logger->set_level(spdlog::level::trace);

    // use this new combined sink as default logger
    spdlog::set_default_logger(combined_logger);
}
