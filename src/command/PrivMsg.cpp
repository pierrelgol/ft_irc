/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivMsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pollivie <plgol.perso@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 18:48:09 by pollivie          #+#    #+#             */
/*   Updated: 2024/11/01 18:48:09 by pollivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command/Command.hpp"

PrivMsg::PrivMsg(Server* server, bool perm) : Command(server, perm) {
        _server = server;
        _perm   = perm;
        log_debug("PrivMsg command initialized");
}

PrivMsg::~PrivMsg() {
        log_debug("PrivMsg command destroyed");
}

// syntax: PRIVMSG <msgtarget> :<message>
void PrivMsg::execute(Client* client, vector<string> args) {
        log_debug("Executing PRIVMSG command for client: " + client->get_nickname());

        if (args.size() < 2 || args[0].empty() || args[1].empty()) {
                client->reply(Replies::ERR_NEEDMOREPARAMS(client->get_nickname(), "PRIVMSG"));
                log_debug("Not enough parameters provided; sending ERR_NEEDMOREPARAMS");
                return;
        }

        // Extract the target and the message
        string                   target = args.at(0);
        string                   message;

        // Concatenate the message parts starting from the second argument
        vector<string>::iterator it  = args.begin() + 1;
        vector<string>::iterator end = args.end();

        while (it != end) {
                message.append(*it + " ");
                it++;
        }

        // Remove leading ':' if present
        if (!message.empty() && message.at(0) == ':') {
                message = message.substr(1);
        }

        // Check if the target is a channel
        if (target.at(0) == '#') {
                Channel* channel = client->get_channel();

                // Channel not found
                if (!channel || channel->get_name() != target) {
                        client->reply(Replies::ERR_NOSUCHCHANNEL(client->get_nickname(), target));
                        log_debug("Channel not found; sending ERR_NOSUCHCHANNEL");
                        return;
                }

                // Check if the channel allows external messages
                if (!channel->get_perm_extern_message()) {
                        vector<string> nicknames = channel->get_user_names();
                        // Check if client is in the channel
                        if (std::find(nicknames.begin(), nicknames.end(), client->get_nickname()) == nicknames.end()) {
                                client->reply(Replies::ERR_CANNOTSENDTOCHAN(client->get_nickname(), target));
                                log_debug("Client not in channel; sending ERR_CANNOTSENDTOCHAN");
                                return;
                        }
                }

                // Broadcast message to the channel
                channel->broadcast_msg_filter(Replies::RPL_PRIVMSG(client->get_prefix(), target, message), client);
                log_debug("Broadcasted message to channel: " + target);
                return;
        }

        // If the target is a client
        Client* dest = _server->get_client(target);
        if (!dest) {
                client->reply(Replies::ERR_NOSUCHNICK(client->get_nickname(), target));
                log_debug("Target client not found; sending ERR_NOSUCHNICK");
                return;
        }

        // Send the private message to the destination client
        dest->write_all(Replies::RPL_PRIVMSG(client->get_prefix(), target, message));
        log_debug("Sent private message to client: " + dest->get_nickname());
}
