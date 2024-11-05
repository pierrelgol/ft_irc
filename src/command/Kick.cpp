/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pollivie <plgol.perso@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 18:47:26 by pollivie          #+#    #+#             */
/*   Updated: 2024/11/01 18:47:26 by pollivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command/Command.hpp"

Kick::Kick(Server* server, bool perm) : Command(server, perm) {
        _server = server;
        _perm   = perm;
        log_debug("Kick command created with perm = " + itoa(perm));
}

Kick::~Kick() {
        log_debug("Kick command destroyed");
}

void Kick::execute(Client* client, vector<string> args) {
        log_debug("Executing KICK command for client: " + client->get_nickname());

        if (args.size() < 2) {
                log_debug("KICK command failed: insufficient arguments provided");
                client->reply(Replies::ERR_NEEDMOREPARAMS(client->get_nickname(), "KICK"));
                return;
        }

        string name   = args[0];
        string target = args[1];
        string reason = "No reason specified!";

        if (args.size() >= 3 && (args[2][0] != ':' || args[2].size() > 1)) {
                reason.clear();
                vector<string>::iterator it  = args.begin() + 2; // Start from args[2] for reason
                vector<string>::iterator end = args.end();
                while (it != end) {
                        reason.append(*it + " ");
                        ++it;
                }
                log_debug("KICK reason specified: " + reason);
        }

        Channel* channel = client->get_channel();
        if (!channel || channel->get_name() != name) {
                log_debug("KICK command failed: client not on channel " + name);
                client->reply(Replies::ERR_NOTONCHANNEL(client->get_nickname(), name));
                return;
        }
        log_debug("Client " + client->get_nickname() + " is on channel " + name);

        if (channel->get_root() != client) {
                log_debug("KICK command failed: insufficient privileges for client " + client->get_nickname());
                client->reply(Replies::ERR_CHANOPRIVSNEEDED(client->get_nickname(), name));
                return;
        }
        log_debug("Client " + client->get_nickname() + " has operator privileges on channel " + name);

        Client* dest = _server->get_client(target);
        if (!dest) {
                log_debug("KICK command failed: target client " + target + " not found");
                client->reply(Replies::ERR_NOSUCHNICK(client->get_nickname(), target));
                return;
        }
        log_debug("Target client " + target + " found");

        if (!dest->get_channel() || dest->get_channel() != channel) {
                log_debug("KICK command failed: target client " + target + " not in channel " + name);
                client->reply(Replies::ERR_USERNOTINCHANNEL(client->get_nickname(), dest->get_nickname(), name));
                return;
        }
        log_debug("Target client " + target + " is in channel " + name);

        // If all checks pass, perform the kick
        channel->kick_client(client, dest, reason);
        log_debug("Client " + target + " kicked from channel " + name + " with reason: " + reason);
}
