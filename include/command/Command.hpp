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

// Header guard to prevent multiple inclusions of this header file
#ifndef __COMMAND_HPP__
#define __COMMAND_HPP__

// Forward declaration of the Command class
class Command;

// Including necessary headers
#include "Replies.hpp"        // For server replies handling
#include "network/Server.hpp" // For Server class definition
#include <numeric>            // For numeric algorithms
#include <string>             // For string class
#include <vector>             // For vector container

// Base Command class declaration
class Command {
        private:
                Command();                              // Private default constructor to prevent instantiation
                Command(const Command& rhs);            // Private copy constructor
                Command& operator=(const Command& rhs); // Private copy assignment operator

        protected:
                Server* _server; // Pointer to the server instance
                bool    _perm;   // Flag indicating if permission is required for the command

                // Protected constructor, can only be called by derived classes
                Command(Server* server, bool perm = true) : _server(server), _perm(perm) {
                }

        public:
                // Default virtual destructor
                virtual ~Command() {
                }

                // Method to check if the command requires authentication
                bool perm_required() const {
                        return (_perm); // Returns the permission requirement flag
                }

                // Pure virtual function to execute the command
                virtual void execute(Client* client, const std::vector<std::string> args) = 0;
};

// Specific command classes are derived from Command

// Notice command class
class Notice : public Command {
        private:
                Notice();                              // Private default constructor
                Notice(const Notice& rhs);             // Private copy constructor
                Notice& operator=(const Command& rhs); // Private copy assignment operator

        public:
                explicit Notice(Server* server, bool perm = true); // Constructor with server and permission flag
                void execute(Client* client, const std::vector<std::string> args); // Executes the command
                ~Notice();                                                         // Destructor
};

// PrivMsg command class
class PrivMsg : public Command {
        private:
                PrivMsg();                              // Private default constructor
                PrivMsg(const PrivMsg& rhs);            // Private copy constructor
                PrivMsg& operator=(const Command& rhs); // Private copy assignment operator

        public:
                explicit PrivMsg(Server* server, bool perm = true);                // Constructor
                void execute(Client* client, const std::vector<std::string> args); // Executes the command
                ~PrivMsg();                                                        // Destructor
};

// Part command class
class Part : public Command {
        private:
                Part();                              // Private default constructor
                Part(const Part& rhs);               // Private copy constructor
                Part& operator=(const Command& rhs); // Private copy assignment operator

        public:
                explicit Part(Server* server, bool perm = true);                   // Constructor
                void execute(Client* client, const std::vector<std::string> args); // Executes the command
                ~Part();                                                           // Destructor
};

// Quit command class
class Quit : public Command {
        private:
                Quit();                              // Private default constructor
                Quit(const Quit& rhs);               // Private copy constructor
                Quit& operator=(const Command& rhs); // Private copy assignment operator

        public:
                explicit Quit(Server* server, bool perm = true);                   // Constructor
                void execute(Client* client, const std::vector<std::string> args); // Executes the command
                ~Quit();                                                           // Destructor
};

// Join command class
class Join : public Command {
        private:
                Join();                              // Private default constructor
                Join(const Join& rhs);               // Private copy constructor
                Join& operator=(const Command& rhs); // Private copy assignment operator

        public:
                explicit Join(Server* server, bool perm = true);                   // Constructor
                void execute(Client* client, const std::vector<std::string> args); // Executes the command
                ~Join();                                                           // Destructor
};

// User command class
class User : public Command {
        private:
                User();                              // Private default constructor
                User(const User& rhs);               // Private copy constructor
                User& operator=(const Command& rhs); // Private copy assignment operator

        public:
                User(Server* server, bool perm = true);                            // Constructor
                void execute(Client* client, const std::vector<std::string> args); // Executes the command
                ~User();                                                           // Destructor
};

// Nick command class
class Nick : public Command {
        private:
                Nick();                              // Private default constructor
                Nick(const Nick& rhs);               // Private copy constructor
                Nick& operator=(const Command& rhs); // Private copy assignment operator

        public:
                Nick(Server* server, bool perm = true);                            // Constructor
                void execute(Client* client, const std::vector<std::string> args); // Executes the command
                ~Nick();                                                           // Destructor
};

// Pass command class
class Pass : public Command {
        private:
                Pass();                              // Private default constructor
                Pass(const Pass& rhs);               // Private copy constructor
                Pass& operator=(const Command& rhs); // Private copy assignment operator

        public:
                explicit Pass(Server* server, bool perm = true);                   // Constructor
                void execute(Client* client, const std::vector<std::string> args); // Executes the command
                ~Pass();                                                           // Destructor
};

// Kick command class
class Kick : public Command {
        private:
                Kick();                              // Private default constructor
                Kick(const Kick& rhs);               // Private copy constructor
                Kick& operator=(const Command& rhs); // Private copy assignment operator

        public:
                explicit Kick(Server* server, bool perm = true);                   // Constructor
                void execute(Client* client, const std::vector<std::string> args); // Executes the command
                ~Kick();                                                           // Destructor
};

// Ping command class
class Ping : public Command {
        private:
                Ping();                              // Private default constructor
                Ping(const Ping& rhs);               // Private copy constructor
                Ping& operator=(const Command& rhs); // Private copy assignment operator

        public:
                explicit Ping(Server* server, bool perm = true);                   // Constructor
                void execute(Client* client, const std::vector<std::string> args); // Executes the command
                ~Ping();                                                           // Destructor
};

// Pong command class
class Pong : public Command {
        private:
                Pong();                              // Private default constructor
                Pong(const Pong& rhs);               // Private copy constructor
                Pong& operator=(const Command& rhs); // Private copy assignment operator

        public:
                explicit Pong(Server* server, bool perm = true);                   // Constructor
                void execute(Client* client, const std::vector<std::string> args); // Executes the command
                ~Pong();                                                           // Destructor
};

// Mode command class
class Mode : public Command {
        private:
                Mode();                              // Private default constructor
                Mode(const Mode& rhs);               // Private copy constructor
                Mode& operator=(const Command& rhs); // Private copy assignment operator

        public:
                explicit Mode(Server* server, bool perm = true);                   // Constructor
                void execute(Client* client, const std::vector<std::string> args); // Executes the command
                ~Mode();                                                           // Destructor
};

// Undefine the DEFINE_COMMAND_CLASS macro if it's defined
#undef DEFINE_COMMAND_CLASS

// End of header guard
#endif // __COMMAND_HPP__
