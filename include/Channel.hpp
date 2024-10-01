/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pollivie <pollivie.student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 14:46:26 by pollivie          #+#    #+#             */
/*   Updated: 2024/10/01 14:46:27 by pollivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "BoundedArray.hpp"
#include "Client.hpp"
#include "Common.hpp"

#define CHANNEL_MAX_CLIENT 32

class Channel {
      private:
	Optional<string>				     _chan_name;
	Optional<string>				     _chan_password;
	Optional<string>				     _chan_conf;
	Optional<Client *>				     _chan_sudo;
	Optional<usize>					     _chan_limit;
	bool						     _chan_extern;
	BoundedArray<Optional<Client *>, CHANNEL_MAX_CLIENT> _clients;

	Channel();

      public:
	Channel(const string &name, const string &passowrd, Client *sudo);
	~Channel();

	
};

#endif // CHANNEL_HPP
