/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pollivie <plgol.perso@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 13:24:16 by pollivie          #+#    #+#             */
/*   Updated: 2024/11/01 13:24:17 by pollivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Common.hpp"
#include "network/Server.hpp"
#include <csignal>

void _log_debug(const char* file, const char* function, int line, const std::string& message) {
        // Get current time
        std::time_t t = std::time(0);
        char        time_str[64];
        std::strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", std::localtime(&t));

        // Print formatted log message
        std::cout << COLOR_CYAN << "[ircserv] " << COLOR_GREEN << "[" << time_str << "] " // Timestamp
                  << COLOR_YELLOW << "[" << std::setw(80) << message << "] "              // Right-aligned message
                  << COLOR_BLUE << "in \"" << function << "\" "                           // Function name
                  << "from " << file << ":" << line                                       // File and line
                  << COLOR_RESET << std::endl;
}

string itoa(i64 value) {
        bool   is_negative = (value < 0);
        string result;

        // Handle 0 explicitly
        if (value == 0) {
                return "0";
        }

        // Work with positive values for simplicity
        unsigned int num = (is_negative) ? -value : value;

        // Convert each digit to a character and add it to the result
        while (num > 0) {
                result.insert(result.begin(), '0' + (num % 10));
                num /= 10;
        }

        // Add negative sign if necessary
        if (is_negative) {
                result.insert(result.begin(), '-');
        }

        return result;
}

int main(int argc, char* argv[]) {
        log_debug("Starting ircserv...");

        if (argc != 3) {
                log_debug("Invalid number of arguments");
                eprintln("error : wrong number of arguments.\nusage :\n\n$>./ircserv <port> <password>.");
                return (1);
        }

        string port = string(argv[1]);
        string pass = string(argv[2]);

        log_debug("Received arguments: port = " + port + ", password = " + pass);

        Server* server = NULL; // Use nullptr for better type safety
        signal(SIGINT, server_signal_handler);
        signal(SIGQUIT, server_signal_handler);

        try {
                server = new Server(port, pass);
                log_debug("Server instance created successfully.");
                server->run();
        } catch (const std::exception& e) {
                log_debug("Exception occurred: " + std::string(e.what()));
                eprintln(e.what());
                delete server; // Ensure proper cleanup on exception
                return (1);
        }

        delete server; // Cleanup after run

        log_debug("ircserv terminated successfully.");
        return (0);
}
