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
        std::list<Client>        _clients;
        std::list<Channel>       _channels;
        std::list<struct pollfd> _poll_fds;

      public:
        Server() : _password(""), _port(0), _socket(0) {
        }

        Server(const Server &other) {
                *this = other;
        }

        Server &operator=(const Server &other) {
                if (this != &other) {
                        this->_password = other._password;
                        this->_port     = other._port;
                        this->_socket   = other._socket;
                        this->_clients  = other._clients;
                        this->_channels = other._channels;
                        this->_poll_fds = other._poll_fds;
                }
                return (*this);
        }

        Server(i32 port, const string &password) {
                _port     = port;
                _password = password;
        }

        ~Server() {
        }
};

bool Server::_signal = false;

#endif // SERVER_HPP
