/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pollivie <pollivie.student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 10:16:51 by pollivie          #+#    #+#             */
/*   Updated: 2024/10/04 10:16:52 by pollivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool server_init (Server_t *const server, i32 port, const char *const password) {
        if (!server or port < 1024 or port >= 65536 or !password) {
                return (false);
        }

        signal (SIGINT, server_handle_signal);
        signal (SIGQUIT, server_handle_signal);
        log_info ("server : signal_handler setup.");

        memset (&server->address, 0, sizeof (struct sockaddr_in));
        server->address.sin_family      = AF_INET;
        server->address.sin_addr.s_addr = INADDR_ANY;
        server->address.sin_port        = htons (port);
        log_info ("server : port resolution done.");

        server->password = strdup (password);
        if (!server->password) {
                return (false);
        }
        log_info ("server : password setup.");

        server->socket = socket (AF_INET, SOCK_STREAM, 0);
        if (server->socket == -1) {
                return (false);
        }
        log_info ("server : socket openned.");

        isize error_or_value = 0;
        isize optval         = 0;

        error_or_value = setsockopt (server->socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof (optval));
        if (error_or_value == -1) {
                return (false);
        }
        log_info ("server : socket options setup.");

        error_or_value = fcntl (server->socket, F_SETFL, O_NONBLOCK);
        if (error_or_value == -1) {
                return (false);
        }
        log_info ("server : socket switched to O_NONBLOCK.");

        error_or_value = bind (server->socket, (const struct sockaddr *) &server->address, sizeof (struct sockaddr_in));
        if (error_or_value == -1) {
                return (false);
        }
        log_info ("server : socket binded.");

        error_or_value = listen (server->socket, SOMAXCONN);
        if (error_or_value == -1) {
                return (false);
        }
        log_info ("server : socket is listenning.");

        server->pollfds.push_back ((struct pollfd){
            .fd      = server->socket,
            .events  = POLLIN,
            .revents = 0,
        });

        log_info ("server : initialization is done. waiting for new connections...");
        return (true);
}

bool server_deinit (Server_t *const server) {
        log_info ("server : deinitialization begin.");
        if (server->password) {
                free (server->password);
        }

        if (server->socket != -1) {
                close (server->socket);
        }

        for (ClientIterator_t it = server->clients.begin ( ); it != server->clients.end ( ); it++) {
                client_disconnect (&(*it));
        }

        for (usize i = 0; i < server->pollfds.size ( ); i++) {
                close (server->pollfds[i].fd);
        }

        log_info ("server : deinitialization done.");
        return (true);
}

bool server_run (Server_t *const server) {
        while (!server_should_close ( )) {
                if (poll (&server->pollfds[0], server->pollfds.size ( ), -1) == -1 and !server_should_close ( )) {
                        return (false);
                }

                for (usize i = 0; i < server->pollfds.size ( ); i += 1) {
                        if (!(server->pollfds[i].revents & POLLIN))
                                continue;

                        if (server->pollfds[i].fd == server->socket) {
                                log_info ("server : received a new connection request.");
                                if (server_connect_client (server)) {
                                        log_info ("server : successfully connected the new client.");
                                } else {
                                        log_error ("server : failed to connect the new client.");
                                }
                        } else {
                                log_info ("server : received a new process request.");
                                if (server_process_client (server, server->pollfds[i].fd)) {
                                        log_info ("server : successfully processed the client request.");
                                } else {
                                        log_error ("server : failed to process the client request.");
                                }
                        }
                }
        }
        server_deinit (server);
        return (true);
}

bool server_connect_client (Server_t *const server) {
        struct sockaddr_in new_client_address = (struct sockaddr_in){ };
        Client_t           new_client         = (Client_t){ };
        socklen_t          addr_len           = sizeof (struct sockaddr_in);
        isize              error_or_value     = 0;

        error_or_value = accept (server->socket, (struct sockaddr *) &new_client_address, &addr_len);
        if (error_or_value == -1) {
                return (false);
        }

        new_client.fd  = error_or_value;
        error_or_value = fcntl (new_client.fd, F_SETFL, O_NONBLOCK);
        if (error_or_value == -1) {
                return (false);
        }

        server->pollfds.push_back ((struct pollfd){
            .fd      = new_client.fd,
            .events  = POLLIN,
            .revents = 0,
        });

        server->clients.push_back ((Client_t){
            .fd         = new_client.fd,
            .ip_address = inet_ntoa (new_client_address.sin_addr),
            .buffer     = NULL,
        });

        return (true);
}

Client_t *server_get_client (Server_t *const server, i32 client_fd) {
        ClientIterator_t it  = server->clients.begin ( );
        ClientIterator_t end = server->clients.end ( );

        for (; it != end; it++) {
                if ((*it).fd == client_fd) {
                        return &(*it);
                }
        }
        return (NULL);
}

bool server_remove_client (Server_t *const server, i32 client_fd) {
        {
                ClientIterator_t it  = server->clients.begin ( );
                ClientIterator_t end = server->clients.end ( );

                for (; it != end; it++) {
                        if ((*it).fd == client_fd) {
                                client_clear_internal_buffer (&(*it));
                                close ((*it).fd);
                                server->clients.erase (it);
                                break;
                        }
                }
                if (it == end)
                        return (false);
        }

        {
                PollfdIterator_t it  = server->pollfds.begin ( );
                PollfdIterator_t end = server->pollfds.end ( );

                for (; it != end; it++) {
                        if ((*it).fd == client_fd) {
                                server->pollfds.erase (it);
                        }
                }
                if (it == end)
                        return (false);
        }

        return (true);
}

bool server_process_client (Server_t *const server, i32 client_fd) {
        char      buffer[1024];
        Client_t *client = server_get_client (server, client_fd);

        if (!client) {
                return (false);
        } else {
                memset (buffer, 0, ArraySize (buffer));
        }

        isize rbytes = recv (client->fd, buffer, ArraySize (buffer) - 1, 0);
        if (rbytes <= 0) {
                server_remove_client (server, client_fd);
                return (false);
        } else {
                if (client_append_to_internal_buffer (client, buffer)) {
                        log_info ("%s", client_get_internal_buffer (client));
                }

                const char *const client_buffer = client_get_internal_buffer (client);
                if (strstr (client_buffer, "\r\n") == (client_buffer + std::string::npos)) {
                        return (true);
                }
        }
        return (true);
}

void server_handle_signal (i32 signal) {
        DISCARD (signal);
        log_debug ("server : handled signal!");
        global_server_signal = true;
}

bool server_should_close (void) {
        return (global_server_signal == true);
}
