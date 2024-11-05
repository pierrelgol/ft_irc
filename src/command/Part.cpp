/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pollivie <plgol.perso@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 18:47:50 by pollivie          #+#    #+#             */
/*   Updated: 2024/11/01 18:47:56 by pollivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command/Command.hpp"

Part::Part(Server* server, bool perm) : Command(server, perm) {
        _server = server;
        _perm   = perm;
        log_debug("Part command initialized");
}

Part::~Part() {
        log_debug("Part command destroyed");
}

// syntax: PART <channels> [<message>]
void Part::execute(Client* client, vector<string> args) {
        log_debug("Executing PART command for client: " + client->get_nickname());

        if (args.empty()) {
                client->reply(Replies::ERR_NEEDMOREPARAMS(client->get_nickname(), "PART"));
                log_debug("No channels provided; sending ERR_NEEDMOREPARAMS");
                return;
        }

        string   name    = args[0];
        Channel* channel = _server->get_channel(name);

        if (!channel || !client->get_channel() || client->get_channel()->get_name() != name) {
                client->reply(Replies::ERR_NOSUCHCHANNEL(client->get_nickname(), name));
                log_debug("Channel not found or client not in the channel; sending ERR_NOSUCHCHANNEL");
                return;
        }

        client->disconnect();

        // Optionally log the successful part action
        log_debug("Client " + client->get_nickname() + " parted from channel: " + name);
}
