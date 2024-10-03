/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Optionnal.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pollivie <pollivie.student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 09:08:15 by pollivie          #+#    #+#             */
/*   Updated: 2024/10/03 09:08:16 by pollivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPTIONNAL_HPP
#define OPTIONNAL_HPP

#include <stdexcept>

template <typename T>
struct Optional {
        bool _valid;
        T    _value;

        Optional() : _valid(false), _value(T()) {
        }

        Optional(const T& value) : _valid(true), _value(value) {
        }

        bool safe_to_unwrap() const {
                return (_valid);
        }

        T unwrap() {
                if (!_valid) throw std::runtime_error("panic : attempt to unwrap a null value");
                return (_value);
        }

        const T& unwrap() const {
                if (!_valid) throw std::runtime_error("panic : attempt to unwrap a null value");
                return (_value);
        }

        operator bool() const {
                return (_valid);
        }
};

#endif
