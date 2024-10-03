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

#include "../Common.hpp"
#include "Channel.hpp"
#include "Client.hpp"

#include <list>
#include <vector>

typedef std::vector<Channel>::iterator             chn_itr;
typedef std::vector<Channel>::const_iterator       cchn_itr;
typedef std::vector<Client>::iterator              cli_itr;
typedef std::vector<Client>::const_iterator        ccli_itr;
typedef std::vector<struct pollfd>::iterator       fds_itr;
typedef std::vector<struct pollfd>::const_iterator cfds_itr;

class Server {
      private:
        static bool                _signal;
        string                     _password;
        i32                        _port;
        i32                        _socket;
        struct sockaddr_in         _address;
        struct pollfd              _new_client_fd;
        struct sockaddr_in         _new_client_address;
        std::vector<Client>        _clients;
        std::vector<Channel>       _channels;
        std::vector<struct pollfd> _fd_list;

        const string format(const string &prefix) {
                std::stringstream fmt;
                fmt << prefix << *this;
                return (fmt.str());
        }

        const string format(const string &prefix, const string &suffix) {
                std::stringstream fmt;
                fmt << prefix << *this << suffix;
                return (fmt.str());
        }

      public:
        static void signal_handler(i32 sig) {
                DISCARD(sig);
                Logger::logSuccess("signal intercepted!");
                Server::_signal = true;
        }

        Client *server_get_client(i32 client_fd) {
                cli_itr it = _clients.begin();
                while (it != _clients.end()) {
                        if ((*it).get_fd() == client_fd) return &(*it);
                        it++;
                }
                return (NULL);
        }

        Channel *server_get_channel(const string &name) {
                chn_itr it = _channels.begin();
                while (it != _channels.end()) {
                        if ((*it).get_name() == name) return &(*it);
                        it++;
                }
                return (NULL);
        }

        /////////////////////////////
        // CONSTRUCTOR/DESTRUCTOR ///
        /////////////////////////////

        Server() : _password(""), _port(0), _socket(0), _address(), _new_client_fd(), _new_client_address(), _clients(), _channels(), _fd_list() {
                Logger::logDebug(format("Default constructor called for --> "));
        }

        Server(const Server &other) {
                *this = other;
                Logger::logDebug(format("Copy constructor called for --> "));
        }

        Server &operator=(const Server &other) {
                if (this != &other) {
                        this->_password           = other._password;
                        this->_port               = other._port;
                        this->_socket             = other._socket;
                        this->_address            = other._address;
                        this->_new_client_fd      = other._new_client_fd;
                        this->_new_client_address = other._new_client_address;
                        this->_clients            = other._clients;
                        this->_channels           = other._channels;
                        this->_fd_list            = other._fd_list;
                }
                Logger::logDebug(format("Assignment constructor called for --> "));
                return (*this);
        }

        Server(i32 port, const string &password) {
                std::memset(&_new_client_address, 0, sizeof(struct sockaddr_in));
                std::memset(&_address, 0, sizeof(struct sockaddr_in));
                _port     = port;
                _password = password;
                Logger::logDebug(format("Custom constructor called for --> "));
        }

        ~Server() {
                Logger::logDebug(format("Default destructor called for --> "));
        }

        /////////////////////////////
        // Server Logic           ///
        /////////////////////////////

        void server_init() {
                Logger::logInfo(format("server : is starting --> "));
                i32 error;
                i64 result;

                _address.sin_family      = AF_INET;
                _address.sin_addr.s_addr = INADDR_ANY;
                _address.sin_port        = htons(this->_port);

                result = socket(AF_INET, SOCK_STREAM, 0);
                if (result == -1) {
                        Logger::logError(format("error : failure to create a socket --> "));
                        throw std::exception();
                } else {
                        _socket = (i32) result;
                        Logger::logSuccess(format("server : has opened a socket successfully --> ", " [OK]"));
                }

                result = setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &error, sizeof(error));
                if (result == -1) {
                        Logger::logError(format("error : failure to set options to socket --> "));
                        throw std::exception();
                } else {
                        Logger::logSuccess(format("server : has configured socket properly --> ", " [OK]"));
                }

                result = fcntl(_socket, F_SETFL, O_NONBLOCK);
                if (result == -1) {
                        Logger::logError(format("error : failure to switch to O_NONBLOCK --> "));
                        throw std::exception();
                } else {
                        Logger::logSuccess(format("server : has switched to O_NONBLOCKING --> ", " [OK]"));
                }

                result = bind(_socket, (struct sockaddr *) &_address, sizeof(_address));
                if (result == -1) {
                        Logger::logError(format("error : failure to bind socket --> "));
                        throw std::exception();
                } else {
                        Logger::logSuccess(format("server : has binded succesfully --> ", " [OK]"));
                }

                result = listen(_socket, SOMAXCONN);
                if (result == -1) {
                        Logger::logError(format("error : failure to listen --> "));
                        throw std::exception();
                } else {
                        Logger::logSuccess(format("server : has started to listen succesfully --> ", " [OK]"));
                }

                _new_client_fd.fd      = _socket;
                _new_client_fd.events  = POLLIN;
                _new_client_fd.revents = 0;
                _fd_list.push_back(_new_client_fd);

                Logger::logInfo("Server is waiting on connection...");
                while (!Server::_signal) {
                        if ((poll(&_fd_list[0], _fd_list.size(), -1) == -1) && Server::_signal == false) {
                                Logger::logError(format("error : failure to poll --> "));
                                throw std::exception();
                        }

                        for (usize i = 0; i < _fd_list.size(); i++) {
                                if (_fd_list[i].revents & POLLIN) {
                                        if (_fd_list[i].fd == _socket) {
                                                Logger::logInfo("server : some client want to connect");
                                                server_on_client_connect();
                                        } else {
                                                Logger::logInfo("server : some client has sent a message");
                                                server_on_client_request(_fd_list[i].fd);
                                        }
                                }
                        }
                }
                server_deinit();
        }

        void server_on_client_connect() {
                socklen_t addr_len = sizeof(_new_client_address);

                std::memset(&_new_client_address, 0, sizeof(struct sockaddr_in));
                i32 client_fd = accept(_socket, (struct sockaddr *) &_new_client_address, &addr_len);
                if (client_fd == -1) {
                        Logger::logError(format("error : failure to accept new connection!"));
                        return;
                } else {
                        Logger::logSuccess("server : new connection accepted");
                }

                i32 error = fcntl(client_fd, F_SETFL, O_NONBLOCK);
                if (error == -1) {
                        Logger::logError("error : failure to switch client to O_NONBLOCK");
                        return;
                } else {
                        Logger::logSuccess("server : new connection successfully switched to O_NONBLOCK");
                }

                _new_client_fd.fd      = client_fd;
                _new_client_fd.events  = POLLIN;
                _new_client_fd.revents = 0;
                _fd_list.push_back(_new_client_fd);
                Client new_client(inet_ntoa(_new_client_address.sin_addr), client_fd);
                _clients.push_back(new_client);
                Logger::logSuccess(new_client.connect_msg());
        };

        void server_on_client_request(i32 client_fd) {
                char buffer[1024];
                std::memset(buffer, 0, 1024);

                Client *it = server_get_client(client_fd);
                if (it == NULL) Logger::logError("error : while trying to process client request (client is NULL)");

                isize rbytes = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
                if (rbytes <= 0) {
                        Logger::logError("error : while trying to process client request");
                        remove_client(client_fd);
                } else {
                        // it->write(buffer);
                        Logger::logInfo(buffer);
                        if (it->get_buffer().find_first_of("\r\n") == string::npos) return;
                }
        }

        void remove_client(i32 client_fd) {
                std::stringstream fmt;
                fmt << client_fd;
                Logger::logDebug(format("server : removes client", fmt.str()));
                for (usize i = 0; i < _clients.size(); i++) {
                        if (_clients[i].get_fd() == client_fd) {
                                _clients[i].disconnect_msg();
                                close(_clients[i].get_fd());
                                _clients.erase(_clients.begin() + i);
                        }
                }

                for (usize i = 0; i < _fd_list.size(); i++) {
                        if (_fd_list[i].fd == client_fd) {
                                _fd_list.erase(_fd_list.begin() + i);
                        }
                }
        }

        void server_deinit() {
                for (cli_itr it = _clients.begin(); it != _clients.end(); it++) {
                        (*it).disconnect_msg();
                        close((*it).get_fd());
                }
                if (_socket != -1) {
                        close(_socket);
                }
        }

        friend std::ostream &operator<<(std::ostream &ostream, const Server &self);
};

bool Server::_signal = false;

std::ostream &operator<<(std::ostream &osteam, const Server &self) {
        osteam << "[signal:" << self._signal;
        osteam << "|password:" << self._password;
        osteam << "|port:" << self._port;
        osteam << "|socket:" << self._socket;
        osteam << "|address:" << inet_ntoa(self._address.sin_addr);
        osteam << "]";
        return (osteam);
}
#endif // SERVER_HPP
