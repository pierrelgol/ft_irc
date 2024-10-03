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
        Client() : _ip(0), _fd(-1) {
                Logger::logDebug(format("Default constructor called for --> "));
        }

        Client(const Client &other) {
                *this = other;
                Logger::logDebug(format("Copy constructor called for --> "));
        }

        Client &operator=(const Client &other) {
                if (this != &other) {
                        this->_fd = other._fd;
                        this->_ip = other._ip;
                }
                Logger::logDebug(format("Assignment constructor called for --> "));
                return (*this);
        }

        Client(const string &ip, i32 fd) : _ip(ip), _fd(fd) {
                Logger::logDebug(format("Custom constructor called for --> "));
        }

        ~Client() {
                Logger::logDebug(format("Default destructor called for --> "));
        }

        i32 getFd() const {
                return (this->_fd);
        }

        void setFd(i32 fd) {
                this->_fd = fd;
        }

        string getIp() const {
                return (this->_ip);
        }

        void setIp(const string &ip) {
                this->_ip = ip;
        }

        friend std::ostream &operator<<(std::ostream &ostream, const Client &self);
};

std::ostream &operator<<(std::ostream &osteam, const Client &self) {
        osteam << "Client [ip:" << self.getIp() << "|fd:" << self.getFd() << "]";
        return (osteam);
}

#endif // CLIENT_HPP
