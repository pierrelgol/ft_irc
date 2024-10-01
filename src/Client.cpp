/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pollivie <pollivie.student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 14:22:53 by pollivie          #+#    #+#             */
/*   Updated: 2024/10/01 14:22:53 by pollivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

bool Client::is_valid() const {
	if (is_registered() or is_operator()) return (true);
	if (!_nick_name.safe_to_unwrap() or !Parser::is_nickname(_nick_name.unwrap())) return (false);
	if (!_user_name.safe_to_unwrap()) return (false);
	if (!_host_name.safe_to_unwrap()) return (false);
	if (!_real_name.safe_to_unwrap()) return (false);
	if (!_chan_name.safe_to_unwrap()) return (false);
	if (!_ip.safe_to_unwrap()) return (false);
	if (!_fd.safe_to_unwrap()) return (false);
	return (true);
}

bool Client::is_undefined() const {
	return (_status == Undefined);
}

bool Client::is_connecting() const {
	return (_status == Connecting);
}

bool Client::is_connected() const {
	return (_status == Connected);
}

bool Client::is_registered() const {
	return (_status == Registered);
}

bool Client::is_operator() const {
	return (_status == Operator);
}

bool Client::is_disconnected() const {
	return (_status == Disconnected);
}

Optional<string> Client::get_nick_name() const {
	return (_nick_name);
}

Optional<string> Client::get_user_name() const {
	return (_user_name);
}

Optional<string> Client::get_host_name() const {
	return (_host_name);
}

Optional<string> Client::get_real_name() const {
	return (_real_name);
}

Optional<string> Client::get_chan_name() const {
	return (_chan_name);
}

Optional<string> Client::get_ip() const {
	return (_ip);
}

ClientStatus Client::get_status() const {
	return (_status);
}

void Client::set_nick_name(const string &nick_name) {
	_nick_name = Optional<string>(nick_name);
}

void Client::set_user_name(const string &user_name) {
	_user_name = Optional<string>(user_name);
}

void Client::set_host_name(const string &host_name) {
	_host_name = Optional<string>(host_name);
}

void Client::set_real_name(const string &real_name) {
	_real_name = Optional<string>(real_name);
}

void Client::set_chan_name(const string &chan_name) {
	_chan_name = Optional<string>(chan_name);
}

void Client::set_ip(const string &ip) {
	_ip = Optional<string>(ip);
}

void Client::set_status(ClientStatus status) {
	_status = status;
}

BoundedArray<i8, IRC_MAX_MESSAGE_LEN> const &Client::get_buffer() const {
	return (_buffer);
}
