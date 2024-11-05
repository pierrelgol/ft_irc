/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pollivie <plgol.perso@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 18:47:59 by pollivie          #+#    #+#             */
/*   Updated: 2024/11/01 18:48:00 by pollivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command/Command.hpp"

Pass::Pass(Server* server, bool perm) : Command(server, perm) {
        _server = server;
        _perm   = perm;
        log_debug("Pass command initialized");
}

Pass::~Pass() {
        log_debug("Pass command destroyed");
}

// syntax: PASS <password>
void Pass::execute(Client* client, vector<string> args) {
        log_debug("Executing PASS command for client: " + client->get_nickname());

        if (args.empty()) {
                client->reply(Replies::ERR_NEEDMOREPARAMS(client->get_nickname(), "PASS"));
                log_debug("No password provided; sending ERR_NEEDMOREPARAMS");
                return;
        }

        if (client->get_is_registered()) {
                client->reply(Replies::ERR_ALREADYREGISTERED(client->get_nickname()));
                log_debug("Client already registered; sending ERR_ALREADYREGISTERED");
                return;
        }

        string provided_password = args[0].substr(args[0][0] == ':' ? 1 : 0);
        if (_server->get_password() != provided_password) {
                client->reply(Replies::ERR_PASSWDMISMATCH(client->get_nickname()));
                log_debug("Password mismatch; sending ERR_PASSWDMISMATCH");
                return;
        } else {
                client->set_is_logged(true);
                client->set_is_restricted(true);
                log_debug("Client " + client->get_nickname() + " logged in successfully");
        }
}
