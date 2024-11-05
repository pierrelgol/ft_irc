/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pollivie <plgol.perso@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 13:05:54 by pollivie          #+#    #+#             */
/*   Updated: 2024/11/01 13:05:54 by pollivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SERVER_HPP__
#define __SERVER_HPP__

#include "Common.hpp"
#include <algorithm>
#include <arpa/inet.h>
#include <cerrno>
#include <fcntl.h>
#include <map>
#include <netdb.h>
#include <stdexcept>
#include <string.h>
#include <string>
#include <sys/poll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

class Server;

#include "Channel.hpp"
#include "Client.hpp"
#include "command/Parser.hpp"

typedef enum ServerEvent {
        PollNothing,
        PollHangUp,
        PollInput,
        PollConnection,
} ServerEvent;

class Server {
                typedef vector<pollfd>::iterator           PollfdIter;
                typedef vector<Channel *>::iterator        ChannelIter;
                typedef map<int, Client *>::iterator       ClientIter;

                typedef vector<pollfd>::const_iterator     ConstPollfdIter;
                typedef vector<Channel *>::const_iterator  ConstChannelIter;
                typedef map<int, Client *>::const_iterator ConstClientIter;

        private:
                i32                _socket;   // Server socket descriptor
                u16                _port;     // Port number on which the server listens
                const string       _hostname; // Hostname of the server
                const string       _password; // Password for server authentication

                vector<pollfd>     _pollfds;  // Vector of pollfd structures for polling connections
                vector<Channel *>  _channels; // Vector of channels managed by the server
                map<i32, Client *> _clients;  // Map of connected clients, keyed by their socket descriptor
                Parser            *_parser;

                // Default constructor (private to prevent instantiation without parameters)
                Server() unsafe;

                // Copy constructor (private to prevent copying of Server objects)
                Server(const Server &rhs) unsafe;


        public:
                // Constructor that initializes the server with a port and password
                Server(const string &port, const string &password) unsafe;

                // Destructor to clean up resources
                ~Server() unsafe;

                // Getter for the server password (constant reference for efficiency)
                const string &get_password() const safe;

                // Retrieve a client based on their socket descriptor
                Client       *get_client(i32 client_socket) const safe;

                // Retrieve a client based on their nickname
                Client       *get_client(const string &nickname) const safe;

                Channel      *get_channel(const string &channel_name) const safe;

                // Returns true if the server received a signal.
                bool          server_should_stop() const;

                // Main loop to run the server and manage connections
                bool          run() unsafe;

                // Handle new client connections
                void          handle_client_connection() unsafe;

                // Handle client disconnections
                void          handle_client_disconnection(i32 client_socket) unsafe;

                // Handle incoming messages from clients
                void          handle_client_message(i32 client_socket) unsafe;

                // Read all data from a client socket until it is closed or no data is available
                string        read_all(i32 client_socket) unsafe;

                // Create a new channel with a specified name, key, and root client
                Channel      *create_channel(const string &name, const string &key, Client *root) unsafe;

                ServerEvent   identify_event(pollfd client) safe;

                i32           create_socket() unsafe;
};

#endif //__SERVER_HPP__
