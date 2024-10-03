/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pollivie <pollivie.student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 11:02:34 by pollivie          #+#    #+#             */
/*   Updated: 2024/10/03 11:02:35 by pollivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Common.hpp"

#pragma once
#include "Channel.hpp"
class Channel;

class Client {
      private:
        string _ip;
        i32    _fd;
        string _buffer;

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
        Client() : _ip(0), _fd(-1), _buffer("") {
                Logger::logDebug(format("Default constructor called for --> "));
        }

        Client(const Client &other) {
                *this = other;
                Logger::logDebug(format("Copy constructor called for --> "));
        }

        Client &operator=(const Client &other) {
                if (this != &other) {
                        this->_fd     = other._fd;
                        this->_ip     = other._ip;
                        this->_buffer = other._buffer;
                }
                Logger::logDebug(format("Assignment constructor called for --> "));
                return (*this);
        }

        Client(const string &ip, i32 fd) : _ip(ip), _fd(fd), _buffer("") {
                Logger::logDebug(format("Custom constructor called for --> "));
        }

        ~Client() {
                Logger::logDebug(format("Default destructor called for --> "));
        }

        i32 get_fd() const {
                return (_fd);
        }

        void set_fd(i32 fd) {
                _fd = fd;
        }

        string get_ip() const {
                return (_ip);
        }

        void set_ip(const string &ip) {
                _ip = ip;
        }

        string get_buffer() const {
                return (_buffer);
        }

        void write(string msg) {
                _buffer += msg;
        }

        void clear() {
                _buffer.clear();
        }

        const std::string connect_msg() {
                return (format("client : connected to ircserv"));
        }

        const std::string disconnect_msg() {
                return (format("client : disconnected from ircserv"));
        }

        friend std::ostream &operator<<(std::ostream &ostream, const Client &self);
};

std::ostream &operator<<(std::ostream &osteam, const Client &self) {
        osteam << "Client [ip:" << self.get_ip() << "|fd:" << self.get_fd() << "]";
        return (osteam);
}

#endif // CLIENT_HPP
