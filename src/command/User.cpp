/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pollivie <plgol.perso@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 18:48:16 by pollivie          #+#    #+#             */
/*   Updated: 2024/11/01 18:48:17 by pollivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command/Command.hpp"

User::User(Server* server, bool perm) : Command(server, perm) {
        log_debug("User command initialized.");
}

User::~User() {
        log_debug("User command destroyed.");
}

void User::execute(Client* client, std::vector<std::string> args) {
        log_debug("Executing USER command.");

        if (client->get_is_registered()) {
                log_debug("Client is already registered.");
                client->reply(Replies::ERR_ALREADYREGISTERED(client->get_nickname()));
                return;
        }

        if (args.size() < 4) {
                log_debug("Not enough parameters for USER command.");
                client->reply(Replies::ERR_NEEDMOREPARAMS(client->get_nickname(), "USER"));
                return;
        } else {
                // Set username and real name
                client->set_username(args[0]);
                client->set_realname(args[3]);

                log_debug("Username set: " + args[0]);
                log_debug("Real name set: " + args[3]);

                // Announce the user registration
                client->set_is_restricted(false);
                client->set_is_registered(true);
                client->announce();
                log_debug("User announced: " + client->get_nickname());
        }
}
