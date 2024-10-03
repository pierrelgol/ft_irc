/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pollivie <pollivie.student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 15:27:31 by pollivie          #+#    #+#             */
/*   Updated: 2024/09/28 15:27:31 by pollivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Common.hpp"
#include "IRCServer.hpp"

#include <cstdlib>

int main(int argc, char **argv) {

        if (argc != 3) {
                Logger::logError("ircser : wrong numbers of arguments provided!");
                Logger::logInfo("usage  : >$ ./ircserv <port> <password>");
                return (1);
        }
        std::string password(argv[2]);
        std::string port(argv[1]);
        Server      ircserv(std::atoi(argv[1]), password);

        try {
                // signal(SIGINT, Server::sig_handler);
                // signal(SIGQUIT, Server::sig_handler);
                // ircserv.server_init();
                // ircserv.server_run();
        } catch (const std::exception &e) {
                // ircserv.server_deinit();
                // Logger::logError(e.what());
        }
        Logger::logInfo("server : is closing now!");
        return (0);
}
