/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pollivie <pollivie.student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 11:02:42 by pollivie          #+#    #+#             */
/*   Updated: 2024/10/03 11:02:43 by pollivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "../Common.hpp"

#pragma once
#include "Client.hpp"
class Client;

class Channel {
      private:
        string _name;

      public:
        Channel() : _name("") {
        }

        Channel(const Channel &other) : _name(other._name) {
                *this = other;
        }

        Channel &operator=(const Channel &other) {
                if (this != &other) {
                        this->_name = other._name;
                }
                return (*this);
        }

        Channel(const string &name) {
                _name = name;
        }

        ~Channel() {
        }

        const string &get_name() {
                return (this->_name);
        }

        void setName(const string &name) {
                this->_name = name;
        }
};

#endif // CHANNEL_HPP
