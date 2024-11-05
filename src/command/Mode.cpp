/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pollivie <plgol.perso@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 18:47:29 by pollivie          #+#    #+#             */
/*   Updated: 2024/11/01 18:47:29 by pollivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command/Command.hpp"

Mode::Mode(Server* server, bool perm) : Command(server, perm) {
        _server = server;
        _perm   = perm;
        log_debug("Mode command created with perm = " + itoa(perm));
}

Mode::~Mode() {
        log_debug("Mode command destroyed");
}

void Mode::execute(Client* client, vector<string> args) {
        log_debug("Executing MODE command for client: " + client->get_nickname());

        if (args.size() < 2) {
                log_debug("MODE command failed: insufficient arguments provided");
                client->reply(Replies::ERR_NEEDMOREPARAMS(client->get_nickname(), "MODE"));
                return;
        }

        string target = args.at(0);
        log_debug("MODE command target: " + target + ", flags: " + args[1]);

        Channel* channel = _server->get_channel(target);
        if (!channel) {
                log_debug("MODE command failed: channel " + target + " does not exist");
                client->reply(Replies::ERR_NOSUCHCHANNEL(client->get_nickname(), target));
                return;
        }
        log_debug("Target channel " + target + " exists");

        if (channel->get_root() != client) {
                log_debug(
                    "MODE command failed: client " + client->get_nickname() + " lacks operator privileges on channel " +
                    target
                );
                client->reply(Replies::ERR_CHANOPRIVSNEEDED(client->get_nickname(), target));
                return;
        }
        log_debug("Client " + client->get_nickname() + " has operator privileges on channel " + target);

        usize i  = 0;
        usize j  = 2;
        i8    ch = 0;

        while ((ch = args[1][i])) {
                i8   prev_c = i > 0 ? args[1][i - 1] : '\0';
                bool active = prev_c == '+';

                switch (ch) {
                        case 'n' :
                                {
                                        log_debug("Processing +n/-n mode for channel " + target);
                                        channel->set_perm_extern_message(active);
                                        channel->broadcast_msg_all(Replies::RPL_MODE(
                                            client->get_prefix(), channel->get_name(), (active ? "+n" : "-n"), ""
                                        ));
                                        log_debug(
                                            "External message permission set to " +
                                            string(active ? "enabled" : "disabled")
                                        );
                                        break;
                                }
                        case 'l' :
                                {
                                        if (active && j < args.size()) {
                                                log_debug("Processing +l mode with limit for channel " + target);
                                                channel->set_limit(atoi(args[j].c_str()));
                                                channel->broadcast_msg_all(Replies::RPL_MODE(
                                                    client->get_prefix(), channel->get_name(), "+l", args[j]
                                                ));
                                                log_debug("Limit set to " + args[j]);
                                                j++;
                                        } else if (!active) {
                                                log_debug("Processing -l mode, removing limit for channel " + target);
                                                channel->set_limit(0);
                                                channel->broadcast_msg_all(Replies::RPL_MODE(
                                                    client->get_prefix(), channel->get_name(), "-l", ""
                                                ));
                                                log_debug("Limit removed");
                                        }
                                        break;
                                }
                        case 'k' :
                                {
                                        if (active && j < args.size()) {
                                                log_debug("Processing +k mode with key for channel " + target);
                                                channel->set_key(args[j]);
                                                channel->broadcast_msg_all(Replies::RPL_MODE(
                                                    client->get_prefix(), channel->get_name(), "+k", args[j]
                                                ));
                                                log_debug("Key set to " + args[j]);
                                                j++;
                                        } else if (!active) {
                                                log_debug("Processing -k mode, removing key for channel " + target);
                                                channel->set_key("");
                                                channel->broadcast_msg_all(Replies::RPL_MODE(
                                                    client->get_prefix(), channel->get_name(), "-k", ""
                                                ));
                                                log_debug("Key removed");
                                        }
                                        break;
                                }
                        default : log_debug("Unknown mode flag encountered: " + string(1, ch)); break;
                }
                i++;
        }
}
