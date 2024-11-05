/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pollivie <plgol.perso@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 13:33:09 by pollivie          #+#    #+#             */
/*   Updated: 2024/11/01 13:33:09 by pollivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __CLIENT_HPP__
#define __CLIENT_HPP__

class Client;
typedef struct ClientInfo ClientInfo;

#include "Channel.hpp"
#include "Replies.hpp"
#include "Server.hpp"

struct ClientInfo {
                u64 is_connected : 1;  // Indicates if the client is fully connected and registered
                u64 is_restricted : 1; // +r : Client has restricted privileges
                u64 is_registered : 1; // Client has completed registration process
                u64 is_operator : 1;   // +o : Client has operator privileges
                u64 is_away : 1;       // +a : Client is marked as away
                u64 is_invisible : 1;  // +i : Client is invisible to non-privileged users
                u64 is_logged : 1;     // Client has entered the correct password
                u64 reserved : 57;     // Padding for future expansion, maintains 64 bits
};

class Client {
        private:
                Channel   *_channel;  // The channel the client is currently in
                ClientInfo _info;     // Bitfield struct holding client flags
                u16        _port;     // Port number used by the client
                i32        _socket;   // Client's socket descriptor for communication
                string     _hostname; // Hostname of the client
                string     _nickname; // Nickname chosen by the client
                string     _realname; // Real name of the client (usually set on registration)
                string     _username; // Username used for identification on the server

                Client() unsafe;
                Client(const Client &rhs) unsafe;

        public:
                Client(i32 socket, u16 port, const string &hostname) unsafe;
                ~Client() safe;

                // Returns true if the client is connected and registered
                bool          get_is_connected() const safe;

                // Returns true if the client is logged
                bool          get_is_logged() const safe;

                // Returns true if the client has restricted privileges
                bool          get_is_restricted() const safe;

                // Returns true if the client has completed the registration process
                bool          get_is_registered() const safe;

                // Returns true if the client has operator privileges
                bool          get_is_operator() const safe;

                // Returns true if the client is marked as away
                bool          get_is_away() const safe;

                // Returns true if the client is invisible to non-privileged users
                bool          get_is_invisible() const safe;

                // Returns the socket descriptor of the client
                i32           get_socket() const safe;

                // Returns the port number the client is connected to
                u16           get_port() const safe;

                // Returns a pointer to the channel the client is currently in
                Channel      *get_channel() const safe;

                // Returns the hostname of the client
                const string &get_hostname() const safe;

                // Returns the nickname of the client
                const string &get_nickname() const safe;

                // Returns the full prefix of the client (e.g., "nickname!username@hostname")
                const string  get_prefix() const unsafe;

                // Returns the real name of the client
                const string &get_realname() const safe;

                // Returns the username of the client
                const string &get_username() const safe;

                // Sets the connected status of the client
                void          set_is_connected(bool flag) safe;

                // Sets the logged status of the client
                void          set_is_logged(bool flag) safe;

                // Sets the restricted status of the client
                void          set_is_restricted(bool flag) safe;

                // Sets the registered status of the client
                void          set_is_registered(bool flag) safe;

                // Sets the operator status of the client
                void          set_is_operator(bool flag) safe;

                // Sets the away status of the client
                void          set_is_away(bool flag) safe;

                // Sets the invisibility status of the client
                void          set_is_invisible(bool flag) safe;

                // Sets the client's socket descriptor
                void          set_socket(i32 socket) safe;

                // Sets the client's port number
                void          set_port(u16 port) safe;

                // Assigns the client to a specific channel
                void          set_channel(Channel *channel) safe;

                // Sets the client's hostname
                void          set_hostname(const string &hostname) safe;

                // Sets the client's nickname
                void          set_nickname(const string &nickname) safe;

                // Sets the client's real name
                void          set_realname(const string &realname) safe;

                // Sets the client's username
                void          set_username(const string &username) safe;

                // Sends a message to all channels the client is a part of
                void          write_all(const string &message) const unsafe;

                // Reads all messages sent to the client (or processes incoming data)
                void          reply(const string &message) unsafe;

                void          announce() unsafe;

                // Joins the client to a specific channel
                void          join_channel(Channel *channel) unsafe;

                // Removes the client from a specific channel
                void          quit_channel(Channel *channel) unsafe;

                void          disconnect() unsafe;
};

#endif //__CLIENT_HPP__
