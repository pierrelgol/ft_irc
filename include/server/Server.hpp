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

typedef std::list<Channel>::iterator             chn_itr;
typedef std::list<Channel>::const_iterator       cchn_itr;
typedef std::list<Client>::iterator              cli_itr;
typedef std::list<Client>::const_iterator        ccli_itr;
typedef std::list<struct pollfd>::iterator       fds_itr;
typedef std::list<struct pollfd>::const_iterator cfds_itr;

class Server {
      private:
        static bool              _signal;
        string                   _password;
        i32                      _port;
        i32                      _socket;
        struct sockaddr_in       _address;
        struct pollfd            _new_client_fd;
        struct sockaddr_in       _new_client_address;
        std::list<Client>        _clients;
        std::list<Channel>       _channels;
        std::list<struct pollfd> _fd_list;

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
                _port     = port;
                _password = password;
                Logger::logDebug(format("Custom constructor called for --> "));
        }

        ~Server() {
                Logger::logDebug(format("Default destructor called for --> "));
        }

        /////////////////////////////
        // CONSTRUCTOR/DESTRUCTOR ///
        /////////////////////////////

        friend std::ostream &operator<<(std::ostream &ostream, const Server &self);
};

std::ostream &operator<<(std::ostream &osteam, const Server &self) {
        osteam << "[signal:" << self._signal;
        osteam << "|password:" << self._password;
        osteam << "|port:" << self._port;
        osteam << "|socket:" << self._socket;
        osteam << "|address:" << inet_ntoa(self._address.sin_addr);
        osteam << "]";
        return (osteam);
}

bool Server::_signal = false;

#endif // SERVER_HPP
