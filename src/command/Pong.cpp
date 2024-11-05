/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pong.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pollivie <plgol.perso@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 18:48:06 by pollivie          #+#    #+#             */
/*   Updated: 2024/11/01 18:48:06 by pollivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command/Command.hpp"

Pong::Pong(Server* server, bool perm) : Command(server, perm) {
        _server = server;
        _perm   = perm;
        log_debug("Pong command initialized");
}

Pong::~Pong() {
        log_debug("Pong command destroyed");
}

// syntax: PONG <server1> [<server2>]
void Pong::execute(Client* client, std::vector<std::string> args) {
        log_debug("Executing PONG command for client: " + client->get_nickname());

        if (args.empty()) {
                client->reply(Replies::ERR_NEEDMOREPARAMS(client->get_nickname(), "PONG"));
                log_debug("No arguments provided; sending ERR_NEEDMOREPARAMS");
                return;
        }

        client->write_all(Replies::RPL_PING(client->get_prefix(), args.at(0)));
        log_debug("Sent PONG response to client: " + client->get_nickname() + " for server: " + args.at(0));
}
