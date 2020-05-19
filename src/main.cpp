/**
 * @file   main.cpp
 * @author Carolin Schindler
 * @date   16.05.2020 (creation)
 */

#include <CLI/CLI.hpp>
#include "util/CLIValidator.hpp"
#include <spdlog/spdlog.h>
#include <thread>
#include "AI.hpp"

constexpr unsigned int maxVerbosity = spdlog::level::level_enum::n_levels;
constexpr unsigned int maxDifficulty = 1;

constexpr uint16_t defaultPort = 7007;
constexpr unsigned int defaultVerbosity = 5;
constexpr auto defaultName = "ki017";
constexpr unsigned int defaultDifficulty = maxDifficulty;

int main(int argc, char *argv[]) {
    CLI::App app;

    std::string address;

    std::map<std::string, std::string> additionalOptions;
    std::vector<std::string> keyValueStrings;

    uint16_t port = defaultPort;
    unsigned int verbosity = defaultVerbosity;
    std::string name = defaultName;
    unsigned int difficulty = defaultDifficulty;

    app.add_option("--address,-a", address, "IP address of server to connect to")->required()->check(CLI::validate::IPAddress());
    app.add_option("--x", keyValueStrings, "Additional key value pairs");
    app.add_option("--port,-p", port, "Port of server to connect to")->check(CLI::PositiveNumber);
    app.add_option("--verbosity,-v", verbosity, "Logging verbosity")->check(CLI::Range(maxVerbosity));
    app.add_option("--name,-n", name, "Name of AI to be shown in game")->check(CLI::validate::StringLengthTwo());
    app.add_option("--difficulty,-d", difficulty, "Difficulty of AI")->check(CLI::Range(maxDifficulty));

    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError &e) {
        return app.exit(e);
    }

    // handle difficulty and verbosity == 0
    difficulty = difficulty == 0 ? maxDifficulty : difficulty;
    verbosity = verbosity == 0 ? maxVerbosity : verbosity;

    // arrange key value pairs in a map
    for (unsigned int i = 0; i + 1 < keyValueStrings.size(); i += 2) {
        additionalOptions[keyValueStrings.at(i)] = keyValueStrings.at(i + 1);
    }

    // start ki
    AI ki(address, port, name, verbosity, difficulty, additionalOptions);

    // "stay alive"
    std::this_thread::sleep_until(
            std::chrono::system_clock::now() + std::chrono::hours(std::numeric_limits<int>::max()));

    exit(0);
}