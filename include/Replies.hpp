#ifndef __REPLIES_HPP__
#define __REPLIES_HPP__

#include "Common.hpp"

class Replies {
        public:
                /* Error Responses */
                static string ERR_NOTREGISTERED(const string& msg) {
                        log_debug("451 " + msg + " :You have not registered");
                        return "451 " + msg + " :You have not registered";
                }

                static string ERR_ALREADYREGISTERED(const string& msg) {
                        log_debug("462 " + msg + " :You may not register");
                        return "462 " + msg + " :You may not register";
                }

                static string ERR_PASSWDMISMATCH(const string& msg) {
                        log_debug("464 " + msg + " :Password is incorrect");
                        return "464 " + msg + " :Password is incorrect";
                }

                static string ERR_NONICKNAMEGIVEN(const string& msg) {
                        log_debug("431 " + msg + " :Nickname not given");
                        return "431 " + msg + " :Nickname not given";
                }

                static string ERR_NICKNAMEINUSE(const string& msg) {
                        log_debug("433 " + msg + " " + msg + " :Nickname is already in use");
                        return "433 " + msg + " " + msg + " :Nickname is already in use";
                }

                static string ERR_UNKNOWNCOMMAND(const string& msg, const string& command) {
                        log_debug("421 " + msg + " " + command + " :Unknown command");
                        return "421 " + msg + " " + command + " :Unknown command";
                }

                static string ERR_NEEDMOREPARAMS(const string& msg, const string& command) {
                        log_debug("461 " + msg + " " + command + " :Not enough parameters");
                        return "461 " + msg + " " + command + " :Not enough parameters";
                }

                static string ERR_TOOMANYCHANNELS(const string& msg, const string& channel) {
                        log_debug("405 " + msg + " " + channel + " :You have joined too many channels");
                        return "405 " + msg + " " + channel + " :You have joined too many channels";
                }

                static string ERR_NOTONCHANNEL(const string& msg, const string& channel) {
                        log_debug("442 " + msg + " " + channel + " :You're not on that channel");
                        return "442 " + msg + " " + channel + " :You're not on that channel";
                }

                static string ERR_NOSUCHCHANNEL(const string& msg, const string& channel) {
                        log_debug("403 " + msg + " " + channel + " :No such channel");
                        return "403 " + msg + " " + channel + " :No such channel";
                }

                static string ERR_BADCHANNELKEY(const string& msg, const string& channel) {
                        log_debug("475 " + msg + " " + channel + " :Cannot join channel (+k)");
                        return "475 " + msg + " " + channel + " :Cannot join channel (+k)";
                }

                static string ERR_CHANNELISFULL(const string& msg, const string& channel) {
                        log_debug("471 " + msg + " " + channel + " :Cannot join channel (+l)");
                        return "471 " + msg + " " + channel + " :Cannot join channel (+l)";
                }

                static string ERR_CANNOTSENDTOCHAN(const string& msg, const string& channel) {
                        log_debug("404 " + msg + " " + channel + " :Cannot send to channel");
                        return "404 " + msg + " " + channel + " :Cannot send to channel";
                }

                static string ERR_CHANOPRIVSNEEDED(const string& msg, const string& channel) {
                        log_debug("482 " + msg + " " + channel + " :You're not channel operator");
                        return "482 " + msg + " " + channel + " :You're not channel operator";
                }

                static string ERR_NOSUCHNICK(const string& msg, const string& nickname) {
                        log_debug("401 " + msg + " " + nickname + " :No such nick/channel");
                        return "401 " + msg + " " + nickname + " :No such nick/channel";
                }

                static string ERR_USERNOTINCHANNEL(const string& msg, const string& nickname, const string& channel) {
                        log_debug("441 " + msg + " " + nickname + " " + channel + " :They aren't on that channel");
                        return "441 " + msg + " " + nickname + " " + channel + " :They aren't on that channel";
                }

                static string RPL_WELCOME(const string& msg) {
                        log_debug("001 " + msg + " :Welcome " + msg + " to the ft_irc network");
                        return "001 " + msg + " :Welcome " + msg + " to the ft_irc network";
                }

                static string RPL_NAMREPLY(const string& msg, const string& channel, const string& users) {
                        log_debug("353 " + msg + " = " + channel + " :" + users);
                        return "353 " + msg + " = " + channel + " :" + users;
                }

                static string RPL_ENDOFNAMES(const string& msg, const string& channel) {
                        log_debug("366 " + msg + " " + channel + " :End of /NAMES list.");
                        return "366 " + msg + " " + channel + " :End of /NAMES list.";
                }

                static string RPL_JOIN(const string& msg, const string& channel) {
                        log_debug(":" + msg + " JOIN :" + channel);
                        return ":" + msg + " JOIN :" + channel;
                }

                static string RPL_PART(const string& msg, const string& channel) {
                        log_debug(":" + msg + " PART :" + channel);
                        return ":" + msg + " PART :" + channel;
                }

                static string RPL_PING(const string& msg, const string& token) {
                        log_debug(":" + msg + " PONG :" + token);
                        return ":" + msg + " PONG :" + token;
                }

                static string RPL_PRIVMSG(const string& msg, const string& target, const string& message) {
                        log_debug(":" + msg + " PRIVMSG " + target + " :" + message);
                        return ":" + msg + " PRIVMSG " + target + " :" + message;
                }

                static string RPL_NOTICE(const string& msg, const string& target, const string& message) {
                        log_debug(":" + msg + " NOTICE " + target + " :" + message);
                        return ":" + msg + " NOTICE " + target + " :" + message;
                }

                static string RPL_QUIT(const string& msg, const string& message) {
                        log_debug(":" + msg + " QUIT :Quit: " + message);
                        return ":" + msg + " QUIT :Quit: " + message;
                }

                static string
                RPL_KICK(const string& msg, const string& channel, const string& target, const string& reason) {
                        log_debug(":" + msg + " KICK " + channel + " " + target + " :" + reason);
                        return ":" + msg + " KICK " + channel + " " + target + " :" + reason;
                }

                static string
                RPL_MODE(const string& msg, const string& channel, const string& modes, const string& args) {
                        log_debug(":" + msg + " MODE " + channel + " " + modes + " " + args);
                        return ":" + msg + " MODE " + channel + " " + modes + " " + args;
                }
};

#endif // __REPLIES_HPP__
