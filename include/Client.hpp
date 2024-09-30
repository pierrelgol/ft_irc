/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pollivie <pollivie.student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 09:26:56 by pollivie          #+#    #+#             */
/*   Updated: 2024/09/30 09:26:57 by pollivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Common.hpp"

class Client {
      private:
	uuid _id;
	string	   _ip;
	i32	   _fd;

	static uuid serialize(const string &ip, i32 fd) {
		uuid hash = 5381;

		for (size_t i = 0; i < ip.length(); ++i) {
			hash = ((hash << 5) + hash) + ip[i];
		}
		hash = ((hash << 5) + hash) + fd;
		return (hash);
	}

	string format(const string &prefix, const string &suffix) {
		std::stringstream format;
		format << prefix << "[uuid:" << _id << ":ip:" << _ip << ":fd:" << _fd << "]" << suffix;
		return (format.str());
	}

	string format(const string &prefix) {
		std::stringstream format;
		format << prefix << "[uuid:" << _id << ":ip:" << _ip << ":fd:" << _fd << "]";
		return (format.str());
	}

      public:
	Client() : _id(serialize(CLIENT_DEFAULT_IP, CLIENT_DEFAULT_FD)), _ip(CLIENT_DEFAULT_IP), _fd(CLIENT_DEFAULT_FD) {
		Logger::logDebug(format("Default constructor called --> "));
	}

	Client(const Client &other) : _id(other._id), _ip(other._ip), _fd(other._fd) {
		Logger::logDebug(format("Copy constructor called --> "));
	}

	Client &operator=(const Client &other) {
		if (this != &other) {
			this->_fd = other._fd;
			this->_ip = other._ip;
			this->_id = serialize(other._ip, other._fd);
		}
		return (*this);
	}

	Client(string ip, i32 fd) : _id(serialize(ip, fd)), _ip(ip), _fd(fd) {
		Logger::logDebug(format("Custom constructor called --> "));
	}

	bool operator==(const Client &maybe_self) const {
		return (this->getId() == maybe_self.getId());
	}

	bool operator!=(const Client &maybe_not_self) const {
		return (this->getId() != maybe_not_self.getId());
	}

	i32 getFd() const {
		return (this->_fd);
	}

	string getIp() const {
		return (this->_ip);
	}

	uuid getId() const {
		return (this->_id);
	}

	void setFd(i32 fd) {
		this->_fd = fd;
	}

	void setIp(string ip) {
		this->_ip = ip;
	}

	bool disconnect() {
		if (this->getFd() == CLIENT_DEFAULT_FD) {
			Logger::logWarn(format("client is already disconnected"));
			return (false);
		} else {
			close(this->_fd);
			this->setFd(CLIENT_DEFAULT_FD);
			return (true);
		}
	}

	~Client() {
		Logger::logDebug(format("Default destructor called --> "));
	}

	friend std::ostream &operator<<(std::ostream &ostream, const Client &self);
};

std::ostream &operator<<(std::ostream &osteam, const Client &self) {
	osteam << "Client [ID: " << self.getId() << ", IP: " << self.getIp() << ", FD: " << self.getFd() << "]";
	return (osteam);
}
#endif // CLIENT_HPP
