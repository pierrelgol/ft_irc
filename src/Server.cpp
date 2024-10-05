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

        // we setup  the signal_handler to prvent sigint/sigquit
        // from killing our server.
        signal (SIGINT, server_handle_signal);
        signal (SIGQUIT, server_handle_signal);
        log_info ("server : signal_handler setup.");

        // we setup the port, config
        memset (&server->address, 0, sizeof (struct sockaddr_in));
        server->address.sin_family      = AF_INET;
        server->address.sin_addr.s_addr = INADDR_ANY;
        server->address.sin_port        = htons (port);
        log_info ("server : port resolution done.");

        // clone the password, to make it so that maybe we can change it
        // while the server is running and free it properly, but if we
        // don't need to change the password, we might just make it a
        // const pointer to the argv[2]
        server->password = strdup (password);
        if (!server->password) {
                return (false);
        }
        log_info ("server : password setup.");

        // we create our socket which is an fd over the network
        server->socket = socket (AF_INET, SOCK_STREAM, 0);
        if (server->socket == -1) {
                return (false);
        }
        log_info ("server : socket openned.");

        isize error_or_value = 0;
        isize optval         = 0;

        // configure the socket with SO_REUSEADDR, because otherwise we can't reuse the same socket right away,
        // the OS has by default a delay setup for each socket, where it will keep that socket alive (thus unacessible)
        // after the program has released it, this SO_REUSEADDR overrides that default, such that we don't end up in a 
        // situation where reusing the same port/socket leads to a failure.
        error_or_value = setsockopt (server->socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof (optval));
        if (error_or_value == -1) {
                return (false);
        }
        log_info ("server : socket options setup.");

        // pass our socket file descriptor to fcntl, to change it to O_NONBLOCK
        // for non blocking I/O
        error_or_value = fcntl (server->socket, F_SETFL, O_NONBLOCK);
        if (error_or_value == -1) {
                return (false);
        }
        log_info ("server : socket switched to O_NONBLOCK.");

        // binds our socket with our local address
        error_or_value = bind (server->socket, (const struct sockaddr *) &server->address, sizeof (struct sockaddr_in));
        if (error_or_value == -1) {
                return (false);
        }
        log_info ("server : socket binded.");

        // prepare our socket to passively listen for incomming fd connections
        error_or_value = listen (server->socket, SOMAXCONN);
        if (error_or_value == -1) {
                return (false);
        }
        log_info ("server : socket is listenning.");

        // push ourselfs onto the pollfd array
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

        // we free the password
        if (server->password) {
                free (server->password);
        }

        //close the socket if it's still open
        if (server->socket != -1) {
                close (server->socket);
        }

        // iterate over our clients to ask them to disconnect meaning close their fd, and also free
        // their internal buffer
        for (ClientIterator_t it = server->clients.begin ( ); it != server->clients.end ( ); it++) {
                client_disconnect (&(*it));
        }

        // iterate oover the pollfd structs to close any poll fds
        for (usize i = 0; i < server->pollfds.size ( ); i++) {
                close (server->pollfds[i].fd);
        }

        log_info ("server : deinitialization done.");
        return (true);
}

bool server_run (Server_t *const server) {
        // !(global_server_signal == true)
        while (!server_should_close ( )) {
                // we block until poll gives us a signal that one fd is ready to be handled
                if (poll (&server->pollfds[0], server->pollfds.size ( ), -1) == -1 and !server_should_close ( )) {
                        return (false);
                }

                // we linearly look over each fd for a revents (which is poll mechanism for signaling that one fd needs our attention)
                for (usize i = 0; i < server->pollfds.size ( ); i += 1) {
                        // if there isn't any event for that fd we simply continue
                        if (!(server->pollfds[i].revents & POLLIN))
                                continue;

                        // if there is on our own fd we will try to connect the new client
                        if (server->pollfds[i].fd == server->socket) {
                                log_info ("server : received a new connection request.");
                                if (server_connect_client (server)) {
                                        log_info ("server : successfully connected the new client.");
                                } else {
                                        log_error ("server : failed to connect the new client.");
                                }
                        } else {
                                log_info ("server : received a new process request.");
                                // if not we may have received something from a client
                                if (server_process_client (server, server->pollfds[i].fd)) {
                                        log_info ("server : successfully processed the client request.");
                                } else {
                                        log_error ("server : failed to process the client request.");
                                }
                        }
                }
        }
        // when the server is done working it can safely cleanup
        server_deinit (server);
        return (true);
}

bool server_connect_client (Server_t *const server) {
        struct sockaddr_in new_client_address = (struct sockaddr_in){ };
        Client_t           new_client         = (Client_t){ };
        socklen_t          addr_len           = sizeof (struct sockaddr_in);
        isize              error_or_value     = 0;

        // this functions await connections on the current socket, and maps the clients address to the struct sockaddr_in
        // if it succeed it returns the client fd
        error_or_value = accept (server->socket, (struct sockaddr *) &new_client_address, &addr_len);
        if (error_or_value == -1) {
                return (false);
        }

        new_client.fd  = error_or_value;
        // once again we configure our client's fd to be nonblocking I/O mode with O_NONBLOCK
        error_or_value = fcntl (new_client.fd, F_SETFL, O_NONBLOCK);
        if (error_or_value == -1) {
                return (false);
        }

        // we push back our client's fd onto our pollfd array to be monitored for
        // future events
        server->pollfds.push_back ((struct pollfd){
            .fd      = new_client.fd,
            .events  = POLLIN,
            .revents = 0,
        });

        // we then create a new client struct, which will contain our client's fd
        // it's hostname (inet_ntoa), and it's internal buffer from which we will 
        // parse commands and receive messages
        server->clients.push_back ((Client_t){
            .fd         = new_client.fd,
            .ip_address = inet_ntoa (new_client_address.sin_addr),
            .buffer     = NULL,
        });

        return (true);
}

// iterates over our client's list and returns a pointer to it if the
// client's fd is found NULL otherwise
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

// this function will remove a single clients from both our Client's list
// and also our pollfd's list, properly asking the client to free it's internal
// buffer and also closes it's own file descriptor
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
        // when a client requires our attention, we create a large enough buffer
        // to communicate, the IRC RFC allows the client to send a maximum of 
        // 512 bytes of data, but we allow for more, just for safety
        char      buffer[1024];
        Client_t *client = server_get_client (server, client_fd);

        if (!client) {
                return (false);
        } else {
                // ArraySize(x) --> (sizeof (buffer) / sizeof (buffer[0])
                memset (buffer, 0, ArraySize (buffer));
        }

        // we call recv, with our client's fd, pass it a buffer, and a maximum number of bytes
        // to read, it returns -1 on error, or the number of bytes that were sent by the user
        isize rbytes = recv (client->fd, buffer, ArraySize (buffer) - 1, 0);

        // if ther number of bytes is incorrect we remove the client
        // from our pool of clients
        if (rbytes <= 0) {
                server_remove_client (server, client_fd);
                return (false);
        } else {

                // if not we can join our client's buffer, with it's new message
                if (client_append_to_internal_buffer (client, buffer)) {

                        // this is where we will plug the parsing of commands
                        log_info ("%s", client_get_internal_buffer (client));
                }

                const char *const client_buffer = client_get_internal_buffer (client);
                if (strstr (client_buffer, "\r\n") == (client_buffer + std::string::npos)) {
                        return (true);
                }else {
                        client_clear_internal_buffer(client);
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
