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

#include "Client.hpp"
#include "Common.hpp"

class Server {
      private:
	static bool		   _sig;
	const uuid		   _id;
	const string		   _password;
	i32			   _port;
	i32			   _socket;
	std::vector<Client>	   _clients;
	std::vector<struct pollfd> _poll_fds;
	std::map<uuid, Client>	   _clients_index;

      public:
	Server() : _id(serialize()), _password(SERVER_DEFAULT_PASSWORD), _port(SERVER_DEFAULT_PORT), _socket(SERVER_DEFAULT_SOCKET) {
		Logger::logDebug(format("Default constructor called --> "));
	}

	Server(i32 port, string &password) : _id(serialize()), _password(password), _port(port), _socket(SERVER_DEFAULT_SOCKET) {
		Logger::logDebug(format("Default constructor called --> "));
	}

	static uuid serialize() {
		static uuid id = 0;
		return (id++);
	}

	string format(const string &prefix, const string &suffix) {
		std::stringstream format;
		format << prefix << "[uuid:" << _id << ":port:" << _port << ":passowrd:" << _password << ":socket:" << _socket << ":sig:" << _sig << "]"
		       << suffix;
		return (format.str());
	}

	string format(const string &prefix) {
		std::stringstream format;
		format << prefix << "[uuid:" << _id << ":port:" << _port << ":passowrd:" << _password << ":socket:" << _socket << ":sig:" << _sig << "]";
		return (format.str());
	}

	bool socket_init(struct sockaddr_in &address) {
		address.sin_family	= AF_INET;
		address.sin_port	= htons(_port);
		address.sin_addr.s_addr = INADDR_ANY;
		Logger::logDebug(__PRETTY_FUNCTION__);
		return (true);
	}

	bool socket_open() {
		Logger::logDebug(__PRETTY_FUNCTION__);
		return ((_socket = socket(AF_INET, SOCK_STREAM, SERVER_DEFAULT_PROTOCOL)) != -1);
	}

	bool socket_setup() {
		Logger::logDebug(__PRETTY_FUNCTION__);
		i32 optval = -1;
		return (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(i32)) == 0);
	}

	bool socket_unblock(i32 fd) {
		Logger::logDebug(__PRETTY_FUNCTION__);
		return (fcntl(fd, F_SETFL, O_NONBLOCK) == 0);
	}

	bool socket_bind(struct sockaddr_in &address) {
		Logger::logDebug(__PRETTY_FUNCTION__);
		return (bind(_socket, (struct sockaddr *) &address, sizeof(struct sockaddr_in)) == 0);
	}

	bool socket_listen() {
		Logger::logDebug(__PRETTY_FUNCTION__);
		return (listen(_socket, SOMAXCONN) == 0);
	}

	bool socket_subscribe() {
		Logger::logDebug(__PRETTY_FUNCTION__);
		_poll_fds.push_back((struct pollfd){
		    .fd	     = _socket,
		    .events  = POLLIN,
		    .revents = 0,
		});
		return (true);
	}

	bool socket_accept(struct sockaddr_in &address, i32 &maybe_fd) {
		Logger::logDebug(__PRETTY_FUNCTION__);
		socklen_t size = sizeof(struct sockaddr_in);
		return ((maybe_fd = accept(_socket, (struct sockaddr *) &address, &size)) != -1);
	}

	bool server_add_client(struct sockaddr_in &client_address, i32 client_fd) {
		Logger::logDebug(__PRETTY_FUNCTION__);

		if (client_fd == CLIENT_DEFAULT_FD) {
			Logger::logError("server : rejected connection to client!");
		}

		struct pollfd new_pollfd = (struct pollfd){
		    .fd	     = client_fd,
		    .events  = POLLIN,
		    .revents = 0,
		};

		Client new_client(inet_ntoa(client_address.sin_addr), client_fd);
		_clients.push_back(new_client);
		_poll_fds.push_back(new_pollfd);

		std::stringstream fmt;
		fmt << "server : added a new client --> " << new_client;
		Logger::logSuccess(fmt.str());
		return (true);
	}

	bool server_remove_client(i32 client_fd) {
		Logger::logDebug(__PRETTY_FUNCTION__);
		for (usize i = 0; i < _poll_fds.size(); i++) {
			struct pollfd &client = _poll_fds.at(i);
			if (client.fd == client_fd) {
				_poll_fds.erase(_poll_fds.begin() + i);
			}
		}

		Client *client_ptr = server_get_client_ptr_from_fd(client_fd);
		if (client_ptr != NULL) {
			client_ptr->disconnect();
		}

		return (true);
	}

	bool server_should_handle_event(struct pollfd &maybe_event) {
		return (maybe_event.revents & POLLIN);
	}

	bool server_handle_connection() {
		Logger::logDebug(__PRETTY_FUNCTION__);
		struct sockaddr_in client_address;
		i32		   maybe_fd = CLIENT_DEFAULT_FD;

		if (socket_accept(client_address, maybe_fd)) {
			Logger::logSuccess("server : socket accepted a new connection.....[OK]");
		} else {
			Logger::logError("server : socket failed to accept connection...[KO]");
			return (false);
		}

		if (socket_unblock(maybe_fd)) {
			Logger::logSuccess("server : socket new connection unblocked......[OK]");
		} else {
			Logger::logError("server : socket failed to unblocked client....[KO]");
			return (false);
		}

		return (server_add_client(client_address, maybe_fd));
	}

	bool server_receive_message(i32 from) {
		Logger::logDebug(__PRETTY_FUNCTION__);
		char buffer[SERVER_MAX_MSG_SIZE];
		std::memset(buffer, 0x00, SERVER_MAX_MSG_SIZE);
		isize rbytes = recv(from, buffer, SERVER_MAX_MSG_SIZE - 1, 0);

		if (rbytes <= 0) {
			std::stringstream fmt;
			fmt << "server : client(" << from << ") was disconnected";
			Logger::logError(fmt.str());
			server_remove_client(from);
			return (false);
		} else {
			std::stringstream fmt;
			Logger::logError(fmt.str());
			buffer[rbytes] = '\0';
			fmt << "server : client(" << from << ") send --> " << buffer;
			Logger::logInfo(fmt);
		}
		return (true);
	}

	bool server_received_a_new_connection_event(i32 client_fd) {
		return (client_fd == _socket);
	}

	bool should_close() {
		return Server::_sig == true;
	}

	Client *server_get_client_ptr_from_fd(i32 client_fd) {
		Logger::logDebug(__PRETTY_FUNCTION__);
		for (usize i = 0; i < _clients.size(); i++) {
			Client &client = _clients.at(i);
			if (client.getFd() == client_fd) {
				_clients.erase(_clients.begin() + i);
			}
		}
		return (NULL);
	}

	static void sig_handler(i32 signal) {
		DISCARD(signal);
		Logger::logSuccess("signal handled by server");
		_sig = true;
	}

	bool server_init() {
		Logger::logDebug(__PRETTY_FUNCTION__);
		struct sockaddr_in address;

		if (socket_init(address)) {
			Logger::logSuccess("server : socket initialisation................[OK]");
		} else {
			Logger::logError("server : socket initialisation................[KO]");
			return (false);
		}

		if (socket_open()) {
			Logger::logSuccess("server : socket open..........................[OK]");
		} else {
			Logger::logError("server : socket open..........................[KO]");
			return (false);
		}

		if (socket_setup()) {
			Logger::logSuccess("server : socket setup.........................[OK]");
		} else {
			Logger::logError("server : socket setup.........................[KO]");
			return (false);
		}

		if (socket_unblock(_socket)) {
			Logger::logSuccess("server : socket unblock.......................[OK]");
		} else {
			Logger::logError("server : socket unblock.......................[KO]");
			return (false);
		}

		if (socket_bind(address)) {
			Logger::logSuccess("server : socket binding.......................[OK]");
		} else {
			Logger::logError("server : socket binding.......................[KO]");
			return (false);
		}

		if (socket_listen()) {
			Logger::logSuccess("server : socket listening.....................[OK]");
		} else {
			Logger::logError("server : socket listening.....................[KO]");
			return (false);
		}

		if (socket_subscribe()) {
			Logger::logSuccess("server : socket subscribed....................[OK]");
		} else {
			Logger::logError("server : socket subscribed....................[KO]");
			return (false);
		}
		return (true);
	}

	bool server_run() {
		Logger::logDebug(format("server : "));
		Logger::logSuccess("server : is listenning........................[OK]");

		while (!should_close()) {
			struct pollfd *self	     = &_poll_fds[0];
			usize	       number_of_fds = _poll_fds.size();

			if (poll(self, number_of_fds, SERVER_BLOCK_UNTIL_EVENT) == -1 && !should_close()) {
				Logger::logError("server : panic poll failed!");
				return (false);
			}

			for (usize i = 0; i < _poll_fds.size(); i++) {
				struct pollfd &curr = _poll_fds.at(i);
				if (server_should_handle_event(curr)) {
					if (server_received_a_new_connection_event(curr.fd)) {
						Logger::logInfo("server : received a new connection");
						server_handle_connection();
					} else {
						server_receive_message(curr.fd);
						Logger::logInfo("server : received a new message");
					}
				}
			}
		}
		server_deinit();
		return (true);
	}

	bool server_deinit() {
		Logger::logDebug(__PRETTY_FUNCTION__);
		for (usize i = 0; i < _clients.size(); i++) {
			if (_clients.at(i).disconnect()) {
				Logger::logSuccess("server : disconnected.........................[OK]");
			}
		}
		if (_socket != SERVER_DEFAULT_SOCKET) {
			Logger::logSuccess("server : disconnected.........................[KO]");
			close(_socket);
			_socket = SERVER_DEFAULT_SOCKET;
		}
		return (true);
	}

	~Server() {
		Logger::logDebug(__PRETTY_FUNCTION__);
		Logger::logDebug(format("Default destructor called  --> "));
	}
};

bool Server::_sig = SERVER_DEFAULT_SIGNAL;

#endif // SERVER_HPP
