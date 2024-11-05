/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pollivie <plgol.perso@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 18:47:47 by pollivie          #+#    #+#             */
/*   Updated: 2024/11/01 18:47:47 by pollivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command/Parser.hpp"

Parser::Parser(Server* srv) : _server(srv), _commands() {
        log_debug("Initializing Parser and commands");
        _commands.insert(std::make_pair<string, Command*>(string("PASS"), new Pass(_server, false)));
        _commands.insert(std::make_pair<string, Command*>(string("NICK"), new Nick(_server, false)));
        _commands.insert(std::make_pair<string, Command*>(string("USER"), new User(_server, false)));
        _commands.insert(std::make_pair<string, Command*>(string("QUIT"), new Quit(_server, false)));
        _commands.insert(std::make_pair<string, Command*>(string("PING"), new Ping(_server, true)));
        _commands.insert(std::make_pair<string, Command*>(string("PONG"), new Pong(_server, true)));
        _commands.insert(std::make_pair<string, Command*>(string("JOIN"), new Join(_server, true)));
        _commands.insert(std::make_pair<string, Command*>(string("PART"), new Part(_server, true)));
        _commands.insert(std::make_pair<string, Command*>(string("KICK"), new Kick(_server, true)));
        _commands.insert(std::make_pair<string, Command*>(string("MODE"), new Mode(_server, true)));
        _commands.insert(std::make_pair<string, Command*>(string("PRIVMSG"), new PrivMsg(_server, true)));
        _commands.insert(std::make_pair<string, Command*>(string("NOTICE"), new Notice(_server, true)));
}

Parser::~Parser() {
        log_debug("Destroying Parser and cleaning up commands");
        for (map<string, Command*>::iterator it = _commands.begin(); it != _commands.end(); it++) {
                delete it->second;
        }
}

string Parser::format(const string& str) {
        log_debug("Formatting string: \"" + str + "\"");
        string WHITESPACE = " \n\r\t\f\v";
        string result     = "";

        size_t start = str.find_first_not_of(WHITESPACE);
        if (start != string::npos) {
                result = str.substr(start);
        }

        size_t end = result.find_last_not_of(WHITESPACE);
        if (end != string::npos) {
                result = result.substr(0, end + 1);
        }

        log_debug("Formatted string: \"" + result + "\"");
        return result;
}

void Parser::parse(Client* client, const string& message) {
        log_debug("Parsing message from client: " + client->get_nickname());
        std::stringstream ss(message);
        string            syntax;

        while (getline(ss, syntax)) {
                syntax      = format(syntax);
                string name = syntax.substr(0, syntax.find(' '));

                try {
                        vector<string>    args;
                        std::stringstream line(syntax.substr(name.length() + 1)); // Fix: skip the space after command
                        string            buf;

                        Command*          cmd = _commands.at(name);

                        while (line >> buf) {
                                args.push_back(buf);
                        }

                        log_debug("Command: " + name + " | Arguments: " + itoa(args.size()));

                        if (!client->get_is_registered() && cmd->perm_required()) {
                                client->reply(Replies::ERR_NOTREGISTERED(client->get_nickname()));
                                return;
                        }

                        cmd->execute(client, args);
                } catch (const std::out_of_range&) {
                        client->reply(Replies::ERR_UNKNOWNCOMMAND(client->get_nickname(), name));
                        log_debug("Unknown command: " + name);
                } catch (const std::exception& e) {
                        client->reply(Replies::ERR_UNKNOWNCOMMAND(client->get_nickname(), name));
                        log_debug("Exception while parsing command: " + string(e.what()));
                }
        }
}
