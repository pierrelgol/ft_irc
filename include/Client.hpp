/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pollivie <pollivie.student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 13:39:35 by pollivie          #+#    #+#             */
/*   Updated: 2024/10/01 13:39:36 by pollivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "BoundedArray.hpp"
#include "Common.hpp"
#include "Parser.hpp"

enum ClientStatus {
	Undefined,
	Connecting,
	Connected,
	Registered,
	Operator,
	Disconnected,
};

class Client {
      private:
	Optional<string>		      _nick_name;
	Optional<string>		      _user_name;
	Optional<string>		      _host_name;
	Optional<string>		      _real_name;
	Optional<string>		      _chan_name;
	Optional<string>		      _ip;
	Optional<i32>			      _fd;
	ClientStatus			      _status;
	BoundedArray<i8, IRC_MAX_MESSAGE_LEN> _buffer;

      public:
	Client() : _status(Undefined) {
	}

	bool is_valid() const;

	bool is_undefined() const;
	bool is_connecting() const;
	bool is_connected() const;
	bool is_registered() const;
	bool is_operator() const;
	bool is_disconnected() const;

	Optional<string> get_nick_name() const;
	void		 set_nick_name(const string &nick_name);

	Optional<string> get_user_name() const;
	void		 set_user_name(const string &user_name);

	Optional<string> get_host_name() const;
	void		 set_host_name(const string &host_name);

	Optional<string> get_real_name() const;
	void		 set_real_name(const string &real_name);

	Optional<string> get_chan_name() const;
	void		 set_chan_name(const string &chan_name);

	Optional<string> get_ip() const;
	void		 set_ip(const string &ip);

	ClientStatus get_status() const;
	void	     set_status(ClientStatus status);

	BoundedArray<i8, IRC_MAX_MESSAGE_LEN> const &get_buffer() const;
};

#endif
