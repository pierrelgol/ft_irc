#include "network/Channel.hpp"

Channel::Channel(const string &name, const string &key, Client *root)
    : _perm(), _root(root), _name(name), _key(key), _count(0), _clients(0) {
        _perm.has_limit        = false;
        _perm.limit            = 999;
        _perm.extern_message   = false;
        _perm.has_key          = true;
        _perm.invite_only      = false;
        _perm.moderated        = true;
        _perm.private_channel  = false;
        _perm.secret           = false;
        _perm.topic_protection = false;
        _perm.has_no_root      = false;
        _perm.reserved         = 0;
        log_debug("Channel created with name: " + _name + ", key: " + _key + ", root client set.");
}

Channel::~Channel() {
        log_debug("Channel destroyed: " + _name);
}

const string &Channel::get_name() const {
        log_debug("Retrieving channel name: " + _name);
        return (_name);
}

const string &Channel::get_key() const {
        log_debug("Retrieving channel key for: " + _name);
        return (_key);
}

Client *Channel::get_root() const {
        log_debug("Retrieving root client for channel: " + _name);
        return (_root);
}

usize Channel::get_limit() const {
        log_debug("Retrieving client limit for channel: " + _name);
        return (_perm.limit);
}

usize Channel::get_user_count() const {
        log_debug("Retrieving user count for channel: " + _name);
        return (_count);
}

bool Channel::get_perm_extern_message() const {
        log_debug("Checking external message permission for channel: " + _name);
        return (_perm.extern_message);
}

bool Channel::get_perm_has_limit() const {
        log_debug("Checking if channel has a user limit: " + _name);
        return (_perm.has_limit);
}

bool Channel::get_perm_is_private() const {
        log_debug("Checking if channel is private: " + _name);
        return (_perm.private_channel);
}

bool Channel::get_perm_is_secret() const {
        log_debug("Checking if channel is secret: " + _name);
        return (_perm.secret);
}

bool Channel::get_perm_is_invite_only() const {
        log_debug("Checking if channel is invite-only: " + _name);
        return (_perm.invite_only);
}

bool Channel::get_perm_is_moderated() const {
        log_debug("Checking if channel is moderated: " + _name);
        return (_perm.moderated);
}

bool Channel::get_perm_has_topic_protection() const {
        log_debug("Checking if topic protection is enabled for channel: " + _name);
        return (_perm.topic_protection);
}

bool Channel::get_perm_has_no_root() const {
        log_debug("Checking if channel has no root client: " + _name);
        return (_perm.has_no_root);
}

vector<string> Channel::get_user_names() const {
        log_debug("Retrieving user names for channel: " + _name);
        std::vector<std::string> nicknames(0);
        for (ConstClientIter it = _clients.begin(); it != _clients.end(); it++) {
                Client     *client = *it;
                std::string nick   = (client == _root ? "@" : "") + client->get_nickname();
                nicknames.push_back(nick);
        }
        return (nicknames);
}

void Channel::set_perm_extern_message(bool flag) {
        log_debug("Setting external message permission for channel: " + _name + " to " + (flag ? "true" : "false"));
        _perm.extern_message = flag;
}

void Channel::set_perm_has_limit(bool flag) {
        log_debug("Setting user limit permission for channel: " + _name + " to " + (flag ? "true" : "false"));
        _perm.has_limit = flag;
}

void Channel::set_perm_is_private(bool flag) {
        log_debug("Setting private mode for channel: " + _name + " to " + (flag ? "true" : "false"));
        _perm.private_channel = flag;
}

void Channel::set_perm_is_secret(bool flag) {
        log_debug("Setting secret mode for channel: " + _name + " to " + (flag ? "true" : "false"));
        _perm.secret = flag;
}

void Channel::set_perm_is_invite_only(bool flag) {
        log_debug("Setting invite-only mode for channel: " + _name + " to " + (flag ? "true" : "false"));
        _perm.invite_only = flag;
}

void Channel::set_perm_is_moderated(bool flag) {
        log_debug("Setting moderated mode for channel: " + _name + " to " + (flag ? "true" : "false"));
        _perm.moderated = flag;
}

void Channel::set_perm_has_topic_protection(bool flag) {
        log_debug("Setting topic protection for channel: " + _name + " to " + (flag ? "true" : "false"));
        _perm.topic_protection = flag;
}

void Channel::set_perm_has_no_root(bool flag) {
        log_debug("Setting no-root status for channel: " + _name + " to " + (flag ? "true" : "false"));
        _perm.has_no_root = flag;
}

void Channel::set_key(const string &key) {
        log_debug("Setting new key for channel: " + _name);
        _key = key;
}

void Channel::set_limit(usize limit) {

        log_debug("Setting new user limit for channel: " + _name + " to " + itoa(limit));
        _perm.limit = SET_LIMIT_VALUE(limit);
}

void Channel::set_root(Client *client) {
        log_debug("Setting new root client for channel: " + _name);
        _root = client;
}

void Channel::broadcast_msg_all(const string &message) {
        log_debug("Broadcasting message to all clients in channel: " + _name);
        for (ClientIter it = _clients.begin(); it != _clients.end(); ++it) {
                (*it)->write_all(message);
        }
}

void Channel::broadcast_msg_filter(const string &message, Client *except) {
        log_debug("Broadcasting message to all except one client in channel: " + _name);
        for (ClientIter it = _clients.begin(); it != _clients.end(); ++it) {
                if (*it != except) {
                        (*it)->write_all(message);
                }
        }
}

void Channel::kick_client(Client *emitter, Client *target, const string &reason) {
        log_debug(
            "Kicking client: " + target->get_nickname() + " from channel: " + _name + " by " + emitter->get_nickname()
        );
        broadcast_msg_all(Replies::RPL_KICK(emitter->get_prefix(), _name, target->get_nickname(), reason));
        del_client(target);
}

void Channel::add_client(Client *new_client) {
        if (_perm.has_limit && _clients.size() >= _perm.limit) {
                log_debug(
                    "Failed to add client: " + new_client->get_nickname() + " to channel: " + _name +
                    " - limit reached."
                );
                return;
        }
        log_debug("Adding client: " + new_client->get_nickname() + " to channel: " + _name);
        _clients.push_back(new_client);
}

Client *Channel::get_next_client() const {
        log_debug("Retrieving next client for channel: " + _name);
        if (_clients.empty()) {
                return (NULL);
        }
        return (_clients.at(0));
}

void Channel::del_client(Client *client) {
        log_debug("Removing client: " + client->get_nickname() + " from channel: " + _name);
        ClientIter it = std::find(_clients.begin(), _clients.end(), client);
        if (it != _clients.end()) {
                _clients.erase(it);
        }

        client->set_channel(NULL);
        if (client == _root) {
                Client *new_root = get_next_client();
                if (new_root == NULL) {
                        set_root(NULL);
                        set_perm_has_no_root(true);
                        log_debug("No root client remaining in channel: " + _name);
                } else {
                        log_debug("New root client set in channel: " + _name);
                }
        }
}
