/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pollivie <plgol.perso@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 13:32:16 by pollivie          #+#    #+#             */
/*   Updated: 2024/11/01 13:32:16 by pollivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __CHANNEL_HPP__
#define __CHANNEL_HPP__

#include "Common.hpp"

class Channel;
typedef struct ChannelPerm ChannelPerm;

#include "Client.hpp"
#include "Server.hpp"

#define SET_EXTERN_MESSAGE 0x1ULL
#define SET_HAS_KEY 0x2ULL
#define SET_HAS_LIMIT 0x4ULL
#define SET_INVITE_ONLY 0x8ULL
#define SET_MODERATED 0x10ULL
#define SET_PRIVATE_CHANNEL 0x20ULL
#define SET_SECRET 0x40ULL
#define SET_TOPIC_PROTECTION 0x80ULL
#define SET_LIMIT_VALUE(x) ((u64)(x & 0x7FFFFFFF) << 8)

struct ChannelPerm {
                u64 extern_message : 1;   // +n : Allow external messages
                u64 has_key : 1;          // +k : Channel requires a key to join
                u64 has_limit : 1;        // +l : Channel has a user limit
                u64 invite_only : 1;      // +i : Channel is invite-only
                u64 moderated : 1;        // +m : Channel is moderated
                u64 private_channel : 1;  // +p : Channel is private (visible but not joinable)
                u64 secret : 1;           // +s : Channel is secret (doesn't show in public lists)
                u64 has_no_root : 1;      // Channel root user is NULL
                u64 topic_protection : 1; // +t : Only operators can change the topic
                u64 limit : 31;           // User limit value (only valid if has_limit is set)
                u64 reserved : 24;        // Padding to make up 64 bits, for future expansion
};

class Channel {
                typedef vector<Client *>::iterator       ClientIter;
                typedef vector<Client *>::const_iterator ConstClientIter;

        private:
                ChannelPerm      _perm;    // Bitfield struct representing channel permissions
                Client          *_root;    // Pointer to the root user or owner of the channel
                string           _name;    // Name of the channel
                string           _key;     // Key required to join the channel, if set
                usize            _count;   // Maximum number of clients allowed in the channel, or 0 if unlimited
                vector<Client *> _clients; // List of clients currently in the channel

                Channel();
                Channel(const Channel &rhs);

        public:
                Channel(const string &name, const string &key, Client *const root) __nonnull();
                ~Channel();

                // Returns the name of the channel.
                const string  &get_name() const;

                // Returns the channel's key or an empty string if no key is set.
                const string  &get_key() const;

                // Returns a pointer to the root user (channel owner).
                Client        *get_root() const;

                // Returns the current client limit, or 0 if no limit is set.
                usize          get_limit() const;
                //
                // Returns the current client limit, or 0 if no limit is set.
                usize          get_user_count() const;

                // Returns a vector containing the nicknames of all clients in the channel.
                vector<string> get_user_names() const;

                // Checks if external messages are permitted from non-channel users.
                bool           get_perm_extern_message() const;

                // Checks if the channel has a limit on the number of active users.
                bool           get_perm_has_limit() const;

                // Checks if the channel is set to private mode.
                bool           get_perm_is_private() const;

                // Checks if the channel is set to secret mode.
                bool           get_perm_is_secret() const;

                // Checks if the channel is invite-only.
                bool           get_perm_is_invite_only() const;

                // Checks if the channel is currently moderated.
                bool           get_perm_is_moderated() const;

                // Checks if only the root user can change the channel topic.
                bool           get_perm_has_topic_protection() const;

                // Checks if only the root user for this channel is null.
                bool           get_perm_has_no_root() const;

                // Sets the ability to receive external messages from non-channel users.
                void           set_perm_extern_message(bool flag);

                // Sets the limit mode, restricting the maximum number of users.
                void           set_perm_has_limit(bool flag);

                // Sets the channel to private mode, limiting visibility.
                void           set_perm_is_private(bool flag);

                // Sets the channel to secret mode, hiding it from lists.
                void           set_perm_is_secret(bool flag);

                // Sets the channel to invite-only mode.
                void           set_perm_is_invite_only(bool flag);

                // Sets the channel to moderated mode, restricting who can speak.
                void           set_perm_is_moderated(bool flag);

                // Sets topic protection, allowing only the root user to edit the topic.
                void           set_perm_has_topic_protection(bool flag);

                // Sets Channel's root presence flag.
                void           set_perm_has_no_root(bool flag);

                // Updates the channel's key if one exists.
                void           set_key(const string &new_key);

                // Updates the channel's root if one exists.
                void           set_root(Client *client);

                // Updates the client limit if one exists.
                void           set_limit(usize new_limit);

                // Sends a message to all active users in the channel.
                void           broadcast_msg_all(const string &message);

                // Sends a message to all active users in the channel, excluding the specified client.
                void           broadcast_msg_filter(const string &message, Client *except);

                // Removes the target client from the channel, with an optional reason, initiated by the emiter.
                void           kick_client(Client *emiter, Client *target, const string &reason);

                // Adds a new client to the list of active clients in the channel.
                void           add_client(Client *new_client);

                // Removes a client from the list of active clients in the channel.
                void           del_client(Client *new_client);

                Client        *get_next_client() const;
};

#endif //__CHANNEL_HPP__
