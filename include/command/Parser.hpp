/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pollivie <plgol.perso@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 14:48:41 by pollivie          #+#    #+#             */
/*   Updated: 2024/11/01 14:48:42 by pollivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __PARSER_HPP__
#define __PARSER_HPP__

#include "Common.hpp"
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

class Parser;
#include "Command.hpp"

class Parser {
        private:
                Server*               _server;
                map<string, Command*> _commands;

                Parser();
                Parser(const Parser& rhs);
                Parser& operator=(const Parser& rhs);

        public:
                Parser(Server* server);
                ~Parser();

                void   parse(Client* client, const string& message);
                string format(const string& str);
};


#endif // __PARSER_HPP__
