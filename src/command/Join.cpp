/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pollivie <plgol.perso@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 18:47:22 by pollivie          #+#    #+#             */
/*   Updated: 2024/11/01 18:47:23 by pollivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command/Command.hpp"

Join::Join(Server* server, bool perm) : Command(server, perm) {
        _server = server;
        _perm   = perm;
        log_debug("Join command created with server reference and permission status.");
}

Join::~Join() {
        log_debug("Join command destroyed.");
}

void Join::execute(Client* client, const vector<string> args) {
        log_debug("Executing JOIN command.");

        // Check if client is registered
        if (!client->get_is_registered()) {
                log_debug("Client is not registered, sending ERR_NOTREGISTERED.");
                client->reply(Replies::ERR_NOTREGISTERED(client->get_nickname()));
                return;
        }

        // Check if enough arguments were provided
        if (args.empty()) {
                log_debug("JOIN command called without sufficient arguments, sending ERR_NEEDMOREPARAMS.");
                client->reply(Replies::ERR_NEEDMOREPARAMS(client->get_nickname(), "JOIN"));
                return;
        }

        string name = args[0];
        string pass = args.size() > 1 ? args[1] : "";
        log_debug("JOIN command received for channel: " + name + " with pass: " + pass);

        // Check if client is already in a channel
        Channel* channel = client->get_channel();
        if (channel) {
                log_debug("Client already in a channel, sending ERR_TOOMANYCHANNELS.");
                client->reply(Replies::ERR_TOOMANYCHANNELS(client->get_nickname(), name));
                return;
        }

        // Attempt to retrieve or create the channel
        channel = _server->get_channel(name);
        if (!channel) {
                log_debug("Channel does not exist; creating new channel: " + name);
                channel = _server->create_channel(name, pass, client);
                if (!channel) {
                        log_debug("Failed to create channel: " + name);
                        client->reply("Error creating channel.");
                        return;
                }
        } else {
                log_debug("Found existing channel: " + name);
        }

        // Check if channel is at limit capacity
        if (channel->get_limit() > 0 && channel->get_user_count() >= channel->get_limit()) {
                log_debug("Channel " + name + " is full, sending ERR_CHANNELISFULL.");
                client->reply(Replies::ERR_CHANNELISFULL(client->get_nickname(), name));
                return;
        }

        // Check if password is required and correct
        if (channel->get_key() != pass) {
                log_debug("Incorrect key for channel " + name + ", sending ERR_BADCHANNELKEY.");
                client->reply(Replies::ERR_BADCHANNELKEY(client->get_nickname(), name));
                return;
        }

        // Successfully joining the channel
        log_debug("Client successfully joining channel " + name + ".");
        client->join_channel(channel);
}
