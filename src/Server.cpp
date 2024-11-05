/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pollivie <plgol.perso@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 15:25:14 by pollivie          #+#    #+#             */
/*   Updated: 2024/11/01 15:25:15 by pollivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "network/Server.hpp"
#include "Common.hpp"
#include <csignal>
#include <cstdlib>

bool sig = false;

void server_signal_handler(i32 signal) {
        sig = true;
        log_debug("signal received" + itoa(signal));
}

bool Server::server_should_stop() const {
        return (sig == true);
}

Server::Server(const string &port, const string &password)
    : _socket(-1), _port(atoi(port.c_str())), _hostname("127.0.0.1"), _password(password), _pollfds(0), _channels(0),
      _clients(), _parser(NULL) {
        log_debug("Server constructor: Initializing server with port=" + port + " and hostname=" + _hostname);
        _socket = create_socket();
        _parser = new Parser(this);
        log_debug("Server constructor: Server initialized with socket=" + itoa(_socket));
}

Server::~Server() {
        log_debug("Server destructor: Cleaning up resources.");

        delete _parser;
        for (ClientIter it = _clients.begin(); it != _clients.end(); ++it) {
                log_debug("Server destructor: Deleting client with fd=" + itoa(it->first));
                delete it->second;
        }
}

bool Server::run() {
        log_debug("Server::run: Starting server run loop.");

        _pollfds.push_back((struct pollfd){.fd = _socket, .events = POLLIN, .revents = 0});

        while (sig == false) {
                i64 pollings = poll(_pollfds.begin().base(), _pollfds.size(), -1);

                if (pollings == -1 && !server_should_stop()) {
                        log_debug("Server::run: Polling error, exiting.");
                        throw std::runtime_error("poll failed");
                }

                if (pollings > 0) {
                        log_debug("Server::run: Polling returned " + itoa(pollings));
                        for (PollfdIter it = _pollfds.begin(); it != _pollfds.end(); ++it) {

                                log_debug("Server::run: Handling events for fd=" + itoa(it->fd));
                                ServerEvent event = identify_event(*it);
                                log_debug("Server::run: Identified event: " + itoa(event));

                                switch (event) {
                                        case PollNothing :    continue;
                                        case PollConnection : handle_client_connection(); break;
                                        case PollInput :      handle_client_message(it->fd); break;
                                        case PollHangUp :     handle_client_disconnection(it->fd); break;
                                        default :             break;
                                }

                                if (event == PollConnection || event == PollHangUp) {
                                        break;
                                }
                        }
                }
        }
        log_debug("Server::run: Server stopped.");
        return true;
}

void Server::handle_client_connection() {
        log_debug("Server::handle_client_connection: Attempting to accept new client.");

        sockaddr_in addr = {};
        socklen_t   len  = sizeof(addr);
        i32         fd   = 0;

        memset(&addr, 0, len);

        fd = accept(_socket, (sockaddr *)&addr, &len);
        if (fd == -1) {
                throw std::runtime_error("server failed to accept new client.");
        }

        log_debug("Server::handle_client_connection: New client accepted with fd=" + itoa(fd));
        if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1) {
                close(fd);
                throw std::runtime_error("server failed to configure new client.");
        }

        char hostname[NI_MAXHOST] = {};
        memset(hostname, 0, arraysize(hostname));

        if (getnameinfo((sockaddr *)&addr, len, hostname, NI_MAXHOST, NULL, 0, NI_NUMERICSERV) == -1) {
                log_debug("Server::handle_client_connection: Failed to resolve client hostname.");
                throw std::runtime_error("server failed to resolve client's hostname");
        }

        Client *new_client = new Client(fd, ntohs(addr.sin_port), hostname);
        _clients.insert(std::make_pair(fd, new_client));
        _pollfds.push_back((struct pollfd){
            .fd      = fd,
            .events  = POLLIN,
            .revents = 0,

        });
        new_client->set_is_connected(true);
        new_client->set_is_restricted(true);
        log_debug(
            "Server::handle_client_connection: Client " + std::string(hostname) + " connected with fd=" + itoa(fd)
        );
}

void Server::handle_client_disconnection(i32 client_fd) {
        log_debug("Server::handle_client_disconnection: Disconnecting client fd=" + itoa(client_fd));

        try {
                Client *client = _clients.at(client_fd);
                if (not client) {
                        throw std::runtime_error("client doesn't exist.");
                } else {
                        client->disconnect();
                        log_debug("Server::handle_client_disconnection: Client disconnected successfully.");
                }

                _clients.erase(client_fd);

                for (PollfdIter it = _pollfds.begin(); it != _pollfds.end(); it++) {
                        if (it->fd == client_fd) {
                                _pollfds.erase(it);
                                close(client_fd);
                                delete client;
                                log_debug("Server::handle_client_disconnection: Client cleanup completed.");
                                break;
                        }
                }

        } catch (std::exception &e) {
                eprintln(e.what());
        }
}

void Server::handle_client_message(i32 client_fd) {
        log_debug("Server::handle_client_message: Reading message from client fd=" + itoa(client_fd));

        try {
                Client *client = _clients.at(client_fd);
                if (client) {
                        string client_message = read_all(client_fd);
                        log_debug("Server::handle_client_message: Received message: " + client_message);
                        _parser->parse(client, client_message);
                }
        } catch (const std::exception &e) {
                eprintln("Exception: " + std::string(e.what()));
        }
}

string Server::read_all(i32 client_fd) {
        log_debug("");

        char   recv_buffer[509 + 2 + 1] = {};
        string clients_message          = string("");

        do {
                memset(recv_buffer, 0, arraysize(recv_buffer));
                if (recv(client_fd, recv_buffer, arraysize(recv_buffer) - 3, 0) == -1 and (errno != EWOULDBLOCK)) {
                        throw std::runtime_error("server failed to read client's message.");
                }
                clients_message.append(recv_buffer);
        } while (clients_message.find("\n", 0) == string::npos);

        return (clients_message);
}

Channel *Server::create_channel(const string &name, const string &key, Client *root) {
        log_debug("");


        todo("check unique");

        Channel *new_channel = new Channel(name, key, root);
        if (not new_channel) {
                throw std::bad_alloc();
        }
        _channels.push_back(new_channel);
        return (new_channel);
}

i32 Server::create_socket() {
        log_debug("");

        sockaddr_in server_addr = {};
        memset(&server_addr, 0x0, sizeof(sockaddr_in));

        server_addr.sin_family      = AF_INET;
        server_addr.sin_addr.s_addr = INADDR_ANY;
        server_addr.sin_port        = htons(_port);

        i32 fd = socket(AF_INET, SOCK_STREAM, 0);
        if (fd == -1) {
                throw std::runtime_error("server failed to acquire socket.");
        }

        i32 optval = 1;
        if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &optval, sizeof(optval)) == -1) {
                throw std::runtime_error("server failed to configure socket options.");
        }

        if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1) {
                throw std::runtime_error("server failed to configure socket options.");
        }

        if (bind(fd, (sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
                throw std::runtime_error("server failed to bind socket.");
        }

        if (listen(fd, SOMAXCONN) == -1) {
                throw std::runtime_error("server failed to listen through socket.");
        }

        return (fd);
}

// Getter for the server password (constant reference for efficiency)
const safe string &Server::get_password() const {
        log_debug("");

        return (_password);
}

// Retrieve a client based on their socket descriptor
safe Client *Server::get_client(i32 client_socket) const {
        log_debug("");


        for (ConstClientIter it = _clients.begin(); it != _clients.end(); it++) {
                if (it->first == client_socket) {
                        return (it->second);
                }
        }
        return (NULL);
}

// Retrieve a client based on their nickname
safe Client *Server::get_client(const string &nickname) const {
        log_debug("");

        for (ConstClientIter it = _clients.begin(); it != _clients.end(); it++) {
                if (nickname.compare(it->second->get_nickname()) == 0) {
                        return (it->second);
                }
        }
        return (NULL);
}

safe Channel *Server::get_channel(const string &channel_name) const {
        log_debug("");

        for (ConstChannelIter it = _channels.begin(); it != _channels.end(); it++) {
                if (channel_name.compare((*it)->get_name()) == 0) {
                        return (*it);
                }
        }
        return (NULL);
}

safe ServerEvent Server::identify_event(pollfd client) {
        log_debug("");

        if (client.revents & POLLERR) {
                return PollNothing; // Handle error events if necessary
        }

        if (client.revents == 0) {
                return PollNothing;
        } else if (client.revents & POLLIN && client.fd == _socket) {
                return PollConnection;
        } else if (client.revents & POLLHUP) {
                return PollHangUp;
        } else if (client.revents & POLLIN) {
                return PollInput;
        } else {
                return PollNothing;
        }
}
