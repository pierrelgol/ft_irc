/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pollivie <plgol.perso@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 17:27:47 by pollivie          #+#    #+#             */
/*   Updated: 2024/11/01 17:27:47 by pollivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "network/Client.hpp"

Client::Client(i32 fd, u16 port, const string &hostname) unsafe : _channel(NULL),
                                                                  _port(port),
                                                                  _socket(fd),
                                                                  _hostname(hostname),
                                                                  _nickname(hostname),
                                                                  _realname(hostname),
                                                                  _username(hostname) {
        _info.is_away       = false;
        _info.is_logged     = false;
        _info.is_connected  = false;
        _info.is_operator   = false;
        _info.is_invisible  = false;
        _info.is_registered = false;
        _info.is_restricted = true;
        _info.reserved      = 0;
        log_debug("Client object initialized.");
}

safe Client::~Client() {
        log_debug("Client object destroyed.");
}

safe bool Client::get_is_connected() const {
        log_debug("Fetching connection status.");
        return (_info.is_connected);
}

safe bool Client::get_is_restricted() const {
        log_debug("Checking if client has restricted privileges.");
        return (_info.is_restricted);
}

safe bool Client::get_is_registered() const {
        log_debug("Checking if client is registered.");
        return (_info.is_registered);
}

safe bool Client::get_is_operator() const {
        log_debug("Checking if client has operator privileges.");
        return (_info.is_operator);
}

safe bool Client::get_is_away() const {
        log_debug("Checking if client is marked as away.");
        return (_info.is_away);
}

safe bool Client::get_is_logged() const {
        log_debug("Checking if client is logged in.");
        return (_info.is_logged);
}

safe bool Client::get_is_invisible() const {
        log_debug("Checking if client is invisible to non-privileged users.");
        return (_info.is_invisible);
}

safe i32 Client::get_socket() const {
        log_debug("Retrieving client's socket descriptor.");
        return (_socket);
}

safe u16 Client::get_port() const {
        log_debug("Retrieving client's port number.");
        return (_port);
}

safe Channel *Client::get_channel() const {
        log_debug("Retrieving client's current channel.");
        return (_channel);
}

const safe string &Client::get_hostname() const {
        log_debug("Retrieving client's hostname.");
        return (_hostname);
}

const safe string &Client::get_nickname() const {
        log_debug("Retrieving client's nickname.");
        return (_nickname);
}

const string Client::get_prefix() const unsafe {
        log_debug("Generating client's full prefix.");
        std::string username = _username.empty() ? "" : "!" + _username;
        std::string hostname = _hostname.empty() ? "" : "@" + _hostname;
        return (_nickname + username + hostname);
}

const safe string &Client::get_realname() const {
        log_debug("Retrieving client's real name.");
        return (_realname);
}

const safe string &Client::get_username() const {
        log_debug("Retrieving client's username.");
        return (_username);
}

safe void Client::set_is_connected(bool flag) {
        log_debug(std::string("Setting connection status to ") + (flag ? "true" : "false") + ".");
        _info.is_connected = flag;
}

safe void Client::set_is_restricted(bool flag) {
        log_debug(std::string("Setting restricted status to ") + (flag ? "true" : "false") + ".");
        _info.is_restricted = flag;
}

safe void Client::set_is_registered(bool flag) {
        log_debug(std::string("Setting registered status to ") + (flag ? "true" : "false") + ".");
        _info.is_registered = flag;
}

safe void Client::set_is_operator(bool flag) {
        log_debug(std::string("Setting operator status to ") + (flag ? "true" : "false") + ".");
        _info.is_operator = flag;
}

safe void Client::set_is_away(bool flag) {
        log_debug(std::string("Setting away status to ") + (flag ? "true" : "false") + ".");
        _info.is_away = flag;
}

safe void Client::set_is_invisible(bool flag) {
        log_debug(std::string("Setting invisibility status to ") + (flag ? "true" : "false") + ".");
        _info.is_invisible = flag;
}

safe void Client::set_is_logged(bool flag) {
        log_debug(std::string("Setting logged status to ") + (flag ? "true" : "false") + ".");
        set_is_restricted(false);
        _info.is_logged = flag;
}

safe void Client::set_socket(i32 socket) {
        log_debug("Updating client's socket descriptor.");
        _socket = socket;
}

safe void Client::set_port(u16 port) {
        log_debug("Updating client's port number.");
        _port = port;
}

safe void Client::set_channel(Channel *channel) {
        log_debug("Setting client to new channel: " + channel->get_name() + " with key: " + channel->get_key());
        _channel = channel;
}

safe void Client::set_hostname(const string &hostname) {
        log_debug("Updating client's hostname to " + hostname);
        _hostname = hostname;
}

safe void Client::set_nickname(const string &nickname) {
        log_debug("Updating client's nickname to " + nickname);
        _nickname = nickname;
}

safe void Client::set_realname(const string &realname) {
        log_debug("Updating client's real name to " + realname);
        _realname = realname;
}

safe void Client::set_username(const string &username) {
        log_debug("Updating client's username to " + username);
        _username = username;
}

void Client::write_all(const string &message) const unsafe {
        log_debug("Sending message to all channels the client is part of.");
        string buffer = message + "\r\n";
        if (send(_socket, buffer.c_str(), buffer.length(), 0) < 0) {
                throw std::runtime_error("server failed to send message to client.");
        }
}

void Client::reply(const string &message) unsafe {
        log_debug("Sending reply message to client.");
        write_all(":" + get_prefix() + " " + message);
}

void Client::join_channel(Channel *channel) unsafe {
        log_debug("Joining client to channel: " + channel->get_name());
        channel->add_client(this);
        set_channel(channel);
}

void Client::announce() unsafe {
        if (!_info.is_connected or !_info.is_logged or !_info.is_registered or _nickname.empty() or _hostname.empty() or
            _realname.empty() or _username.empty()) {
                log_debug("Client registration requirements not met; announcement aborted.");
                return;
        }
        log_debug("Announcing client as registered.");
        this->set_is_registered(true);
        reply(Replies::RPL_WELCOME(_nickname));
}

void Client::disconnect() unsafe {
        if (!_channel) {
                log_debug("Client has no channel to disconnect from.");
                return;
        }

        const string name = _channel->get_name();
        log_debug("Client disconnecting from channel: " + name);
        _channel->broadcast_msg_all(Replies::RPL_PART(get_prefix(), _channel->get_name()));
        _channel->del_client(this);
}
