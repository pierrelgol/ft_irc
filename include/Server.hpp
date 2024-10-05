/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pollivie <pollivie.student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 09:26:49 by pollivie          #+#    #+#             */
/*   Updated: 2024/09/30 09:26:49 by pollivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Common.hpp"

#include <vector>

#ifndef DEBUG_MODE
#define DEBUG_MODE 1
#endif

typedef struct Client  Client_t;
typedef struct Channel Channel_t;
typedef struct Server  Server_t;

typedef std::vector<Client_t>::iterator      ClientIterator_t;
typedef std::vector<Channel_t>::iterator     ChannelIterator_t;
typedef std::vector<struct pollfd>::iterator PollfdIterator_t;

// The client struct will eventually need more stuff in it but to begin 
// it only need a fd, hostname and a buffer to be functionning
struct Client {
        i32   fd;
        char *ip_address;
        char *buffer;
};

bool        client_append_to_internal_buffer (Client_t *const client, const char *const buffer);
const char *client_get_internal_buffer (Client_t *const client);
bool        client_clear_internal_buffer (Client_t *client);
bool        client_disconnect (Client_t *client);

struct Channel {
        char *name;
};

struct Server {
        i32                        socket;
        struct sockaddr_in         address;
        char                      *password;
        std::vector<Client_t>      clients;
        std::vector<Channel_t>     channels;
        std::vector<struct pollfd> pollfds;
};

bool      server_init (Server_t *const server, i32 port, const char *const password);
bool      server_deinit (Server_t *const server);
bool      server_run (Server_t *const server);
bool      server_connect_client (Server_t *const server);
bool      server_process_client (Server_t *const server, i32 client_fd);
bool      server_remove_client (Server_t *const server, i32 client_fd);
void      server_handle_signal (i32 signal);
bool      server_should_close (void);
Client_t *server_get_client (Server_t *const server, i32 client_fd);


#pragma once
extern bool global_server_signal;

#endif        // SERVER_HPP
