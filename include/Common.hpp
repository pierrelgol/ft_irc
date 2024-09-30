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
#include <csignal>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <map>
#include <map>
#include <netinet/in.h>
#include <poll.h>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

const std::string IRCCOLOR = "\033[34m";
const std::string RESET	   = "\033[0m";

#define DISCARD(x) ((void) (x))

#define foreach(type, var, container) for (type::iterator var = container.begin(); var != container.end(); ++var)

#define CLIENT_DEFAULT_IP "null"
#define CLIENT_DEFAULT_FD -1

#define SERVER_DEFAULT_PORT	 0
#define SERVER_DEFAULT_PASSWORD "42"
#define SERVER_DEFAULT_SOCKET	 -1
#define SERVER_DEFAULT_SIGNAL	 false
#define SERVER_DEFAULT_PROTOCOL	 0
#define SERVER_BLOCK_UNTIL_EVENT -1
#define SERVER_MAX_MSG_SIZE	 2048
#define POLL_DEFAULT_REVENTS	 0

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

#endif
