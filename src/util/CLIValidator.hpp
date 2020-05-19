//
// Created by Carolin on 19.05.2020.
//

#ifndef KICLIENT_CLIVALIDATOR_HPP
#define KICLIENT_CLIVALIDATOR_HPP

#include <arpa/inet.h>

namespace CLI::validate {

    class StringLengthTwo : public Validator {
        public:
            StringLengthTwo() : Validator("StrLengthTwo") {
                func_ = [](std::string &str) {
                    if (str.length() < 2) {
                        return std::string("String has to contain min. two characters: ") + str + ')';
                    }
                    return std::string();
                };
            }
    };

    class IPAddress : public Validator {
        public:
            IPAddress() : Validator("IPV4 or IPV6") {
                func_ = [](const std::string &str) {
                    if (str.find('.') != std::string::npos) {
                        // ipv4
                        struct sockaddr_in sa{};
                        if (inet_pton(AF_INET, str.c_str(), &(sa.sin_addr)) == 0) {
                            return std::string("Invalid IPV4 address (") + str + ')';
                        }
                        return std::string();
                    }
                    if (str.find(':') != std::string::npos) {
                        // ipv6
                        struct sockaddr_in6 sa{};
                        if (inet_pton(AF_INET6, str.c_str(), &(sa.sin6_addr)) == 0) {
                            return std::string("Invalid IPV6 address (") + str + ')';
                        }
                        return std::string();
                    }
                    return std::string("Invalid IP address must contain . or : (") + str + ')';
                };
            }
    };
}


#endif //KICLIENT_CLIVALIDATOR_HPP
