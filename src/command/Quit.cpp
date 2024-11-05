/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pollivie <plgol.perso@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 18:48:13 by pollivie          #+#    #+#             */
/*   Updated: 2024/11/01 18:48:14 by pollivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command/Command.hpp"

Quit::Quit(Server* server, bool perm) : Command(server, perm) {
        _server = server;
        _perm   = perm;
        log_debug("Quit command initialized");
}

Quit::~Quit() {
        log_debug("Quit command destroyed");
}

// syntax: QUIT [<message>]
void Quit::execute(Client* client, vector<string> args) {
        log_debug("Executing QUIT command for client: " + client->get_nickname());

        // Determine the quit reason, defaulting to "Leaving..." if none is provided
        string reason = args.empty() ? "Leaving..." : args.at(0);

        // Remove leading ':' if present
        if (!reason.empty() && reason.at(0) == ':') {
                reason = reason.substr(1);
        }

        // Notify other clients about the disconnection
        client->write_all(Replies::RPL_QUIT(client->get_prefix(), reason));
        log_debug("Client " + client->get_nickname() + " has quit: " + reason);

        // Handle the client's disconnection
        _server->handle_client_disconnection(client->get_socket());
        log_debug("Handled disconnection for client: " + client->get_nickname());
}
