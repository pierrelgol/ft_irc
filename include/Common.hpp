/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Common.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pollivie <pollivie.student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 10:00:10 by pollivie          #+#    #+#             */
/*   Updated: 2024/09/30 10:00:10 by pollivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMON_HPP
#define COMMON_HPP

#include "Logger.hpp"

#include <arpa/inet.h>
#include <cassert>
#include <cctype>
#include <csignal>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <netinet/in.h>
#include <new>
#include <poll.h>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#ifndef DEBUG_BUILD
#	define DEBUG_BUILD 1
#endif

const std::string BLUE	= "\033[34m";
const std::string RESET = "\033[0m";

#define DISCARD(x) ((void) (x))

typedef char		  i8;
typedef short		  i16;
typedef int		  i32;
typedef long int	  i64;
typedef long int	  isize;
typedef unsigned char	  u8;
typedef unsigned short	  u16;
typedef unsigned int	  u32;
typedef unsigned long int u64;
typedef unsigned long int usize;
typedef float		  f32;
typedef double		  f64;
typedef std::string	  string;
typedef unsigned long int uuid;

#define IRC_MAX_MESSAGE_LEN 512

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
#if (DEBUG_BUILD == 1)
		assert(_valid);
#endif
		if (!_valid) throw std::runtime_error("panic : attempt to unwrap a null value");
		return (_value);
	}

	const T& unwrap() const {
#if (DEBUG_BUILD == 1)
		assert(_valid);
#endif
		if (!_valid) throw std::runtime_error("panic : attempt to unwrap a null value");
		return (_value);
	}

	operator bool() const {
		return (_valid);
	}
};


#endif
