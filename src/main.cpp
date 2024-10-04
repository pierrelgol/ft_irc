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
#include "Server.hpp"

bool global_server_signal = false;

int main (int argc, char **argv) {
        if (argc != 3) {
                log_error ("ircser : wrong numbers of arguments provided!");
                log_info ("usage  : >$ ./ircserv <port> <password>");
                return (1);
        }

        Server_t    server   = (Server_t){ };
        const i32   port     = atoi (argv[1]);
        const char *password = argv[2];

        if (!server_init (&server, port, password)) {
                log_error ("server : initialization failed!");
                server_deinit (&server);
                return (1);
        }

        if (!server_run (&server)) {
                log_error ("server : runtime failure!");
                server_deinit (&server);
                return (1);
        }

        return (0);
}
