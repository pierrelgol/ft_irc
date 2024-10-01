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

#include "BoundedArray.hpp"
#include "Client.hpp"
#include "Common.hpp"

#define SERVER_MAX_CAPACITY 32

// @RFC2812 1.1
class Server {
      private:
	static bool					 _signal;
	Optional<string>				 _name; // max length of 63
	Optional<string>				 _password;
	Optional<i32>					 _port;
	Optional<i32>					 _socket;
	BoundedArray<Client, SERVER_MAX_CAPACITY>	 _clients_info;
	BoundedArray<struct pollfd, SERVER_MAX_CAPACITY> _clients_pollfd;

	typedef BoundedArray<Client, SERVER_MAX_CAPACITY>::iterator	   _clients_info_iter;
	typedef BoundedArray<struct pollfd, SERVER_MAX_CAPACITY>::iterator _clients_pollfd_iter;

      public:
};

#endif // SERVER_HPP
