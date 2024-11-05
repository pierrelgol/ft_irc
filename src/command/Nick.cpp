/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pollivie <plgol.perso@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 18:47:34 by pollivie          #+#    #+#             */
/*   Updated: 2024/11/01 18:47:34 by pollivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "command/Command.hpp"

Nick::Nick(Server* server, bool perm) : Command(server, perm) {
        _server = server;
        _perm   = perm;
        log_debug("Nick command created with perm = " + itoa(perm));
}

Nick::~Nick() {
        log_debug("Nick command destroyed");
}

void Nick::execute(Client* client, vector<string> args) {
        log_debug("Executing NICK command for client: " + client->get_nickname());

        if (args.empty() || args[0].empty()) {
                log_debug("NICK command failed: no nickname provided");
                client->reply(Replies::ERR_NONICKNAMEGIVEN(client->get_nickname()));
                return;
        }

        string nickname = args[0];
        log_debug("Client " + client->get_nickname() + " is attempting to set nickname: " + nickname);

        if (_server->get_client(nickname)) {
                log_debug("NICK command failed: nickname " + nickname + " is already in use");
                client->reply(Replies::ERR_NICKNAMEINUSE(client->get_nickname()));
                return;
        }

        log_debug("Setting nickname for client from " + client->get_nickname() + " to " + nickname);
        client->set_nickname(nickname);
        log_debug("Nickname changed successfully to: " + nickname);
}
