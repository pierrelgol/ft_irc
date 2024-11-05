/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pollivie <plgol.perso@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 18:48:03 by pollivie          #+#    #+#             */
/*   Updated: 2024/11/01 18:48:03 by pollivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command/Command.hpp"

Ping::Ping(Server* server, bool perm) : Command(server, perm) {
        _server = server;
        _perm   = perm;
        log_debug("Ping command initialized");
}

Ping::~Ping() {
        log_debug("Ping command destroyed");
}

// syntax: PING <server1> [<server2>]
void Ping::execute(Client* client, vector<string> args) {
        log_debug("Executing PING command for client: " + client->get_nickname());

        if (args.empty()) {
                client->reply(Replies::ERR_NEEDMOREPARAMS(client->get_nickname(), "PING"));
                log_debug("No arguments provided; sending ERR_NEEDMOREPARAMS");
                return;
        }

        client->write_all(Replies::RPL_PING(client->get_prefix(), args.at(0)));
        log_debug("Sent PING response to client: " + client->get_nickname() + " for server: " + args.at(0));
}
