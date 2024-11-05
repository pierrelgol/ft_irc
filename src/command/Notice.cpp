/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Notice.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pollivie <plgol.perso@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 18:47:37 by pollivie          #+#    #+#             */
/*   Updated: 2024/11/01 18:47:38 by pollivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command/Command.hpp"

Notice::Notice(Server* server, bool perm) : Command(server, perm) {
        _server = server;
        _perm   = perm;
        log_debug("Notice command created with perm = " + itoa(perm));
}

Notice::~Notice() {
        log_debug("Notice command destroyed");
}

void Notice::execute(Client* client, vector<string> args) {
        log_debug("Executing NOTICE command for client: " + client->get_nickname());

        if (args.size() < 2 || args[0].empty() || args[1].empty()) {
                log_debug("NOTICE command failed: insufficient parameters");
                client->reply(Replies::ERR_NEEDMOREPARAMS(client->get_nickname(), "NOTICE"));
                return;
        }

        // Extract the target and construct the message
        string target = args.at(0);
        string message;

        // Construct the message from remaining arguments
        for (size_t i = 1; i < args.size(); ++i) {
                message.append(args[i] + " ");
        }

        if (!message.empty() && message.at(0) == ':') {
                message = message.substr(1);
        }

        log_debug("NOTICE target: " + target + ", message: " + message);

        // If notice is for a channel
        if (target.at(0) == '#') {
                Channel* channel = client->get_channel();

                // Channel not found
                if (!channel) {
                        log_debug("Channel " + target + " not found for client " + client->get_nickname());
                        client->reply(Replies::ERR_NOSUCHCHANNEL(client->get_nickname(), target));
                        return;
                }

                // Check if external messages are allowed
                if (!channel->get_perm_extern_message()) {
                        log_debug("External messages not allowed in channel " + target);

                        // Check if the client is in the channel
                        if (find(
                                channel->get_user_names().begin(),
                                channel->get_user_names().end(),
                                client->get_nickname()
                            ) == channel->get_user_names().end()) {
                                log_debug("Client " + client->get_nickname() + " not in channel " + target);
                                client->reply(Replies::ERR_CANNOTSENDTOCHAN(client->get_nickname(), target));
                                return;
                        }
                }

                // Broadcast the message to the channel
                channel->broadcast_msg_filter(Replies::RPL_NOTICE(client->get_prefix(), target, message), client);
                log_debug("Broadcasted NOTICE to channel " + target);
                return;
        }

        // Else, if notice is for a client
        Client* dest = _server->get_client(target);
        if (!dest) {
                log_debug("Client " + target + " not found");
                client->reply(Replies::ERR_NOSUCHNICK(client->get_nickname(), target));
                return;
        }

        // Send the message to the target client
        dest->write_all(Replies::RPL_NOTICE(client->get_prefix(), target, message));
        log_debug("Sent NOTICE to client " + target);
}
