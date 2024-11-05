/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pollivie <plgol.perso@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 14:50:42 by pollivie          #+#    #+#             */
/*   Updated: 2024/11/01 14:50:42 by pollivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __COMMAND_HPP__
#define __COMMAND_HPP__

class Command;

#include "Replies.hpp"
#include "network/Server.hpp"
#include <numeric>
#include <string>
#include <vector>

class Command {
        private:
                Command();
                Command(const Command& rhs);
                Command& operator=(const Command& rhs);

        protected:
                Server* _server; // Pointer to the server instance
                bool    _perm;   // Authentication requirement flag

                // Protected constructor prevents direct instantiation of Command
                Command(Server* server, bool perm = true) : _server(server), _perm(perm) {
                }

        public:
                // Default virtual destructor
                virtual ~Command() {
                }

                // Returns true if authentication is required to execute this command
                bool perm_required() const {
                        return (_perm);
                }

                // Pure virtual function to execute the command, making Command an abstract class
                virtual void execute(Client* client, const vector<string> args) = 0;
};

// Define each specific command using the macro
class Notice : public Command {
        private:
                Notice();
                Notice(const Notice& rhs);
                Notice& operator=(const Command& rhs);

        public:
                explicit Notice(Server* server, bool perm = true);
                void execute(Client* client, const vector<string> args);
                ~Notice();
};

class PrivMsg : public Command {
        private:
                PrivMsg();
                PrivMsg(const PrivMsg& rhs);
                PrivMsg& operator=(const Command& rhs);

        public:
                explicit PrivMsg(Server* server, bool perm = true);
                void execute(Client* client, const vector<string> args);
                ~PrivMsg();
};

class Part : public Command {
        private:
                Part();
                Part(const Part& rhs);
                Part& operator=(const Command& rhs);

        public:
                explicit Part(Server* server, bool perm = true);
                void execute(Client* client, const vector<string> args);
                ~Part();
};

class Quit : public Command {
        private:
                Quit();
                Quit(const Quit& rhs);
                Quit& operator=(const Command& rhs);

        public:
                explicit Quit(Server* server, bool perm = true);
                void execute(Client* client, const vector<string> args);
                ~Quit();
};

class Join : public Command {
        private:
                Join();
                Join(const Join& rhs);
                Join& operator=(const Command& rhs);

        public:
                explicit Join(Server* server, bool perm = true);
                void execute(Client* client, const vector<string> args);
                ~Join();
};

class User : public Command {
        private:
                User();
                User(const User& rhs);
                User& operator=(const Command& rhs);

        public:
                User(Server* server, bool perm = true);
                void execute(Client* client, const vector<string> args);
                ~User();
};

class Nick : public Command {
        private:
                Nick();
                Nick(const Nick& rhs);
                Nick& operator=(const Command& rhs);

        public:
                Nick(Server* server, bool perm = true);
                void execute(Client* client, const vector<string> args);
                ~Nick();
};

class Pass : public Command {
        private:
                Pass();
                Pass(const Pass& rhs);
                Pass& operator=(const Command& rhs);

        public:
                explicit Pass(Server* server, bool perm = true);
                void execute(Client* client, const vector<string> args);
                ~Pass();
};

class Kick : public Command {
        private:
                Kick();
                Kick(const Kick& rhs);
                Kick& operator=(const Command& rhs);

        public:
                explicit Kick(Server* server, bool perm = true);
                void execute(Client* client, const vector<string> args);
                ~Kick();
};

class Ping : public Command {
        private:
                Ping();
                Ping(const Ping& rhs);
                Ping& operator=(const Command& rhs);

        public:
                explicit Ping(Server* server, bool perm = true);
                void execute(Client* client, const vector<string> args);
                ~Ping();
};

class Pong : public Command {
        private:
                Pong();
                Pong(const Pong& rhs);
                Pong& operator=(const Command& rhs);

        public:
                explicit Pong(Server* server, bool perm = true);
                void execute(Client* client, const vector<string> args);
                ~Pong();
};

class Mode : public Command {
        private:
                Mode();
                Mode(const Mode& rhs);
                Mode& operator=(const Command& rhs);

        public:
                explicit Mode(Server* server, bool perm = true);
                void execute(Client* client, const vector<string> args);
                ~Mode();
};

#undef DEFINE_COMMAND_CLASS

#endif // __COMMAND_HPP__
