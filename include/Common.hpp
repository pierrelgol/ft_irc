/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Common.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pollivie <plgol.perso@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 13:08:33 by pollivie          #+#    #+#             */
/*   Updated: 2024/11/01 13:08:34 by pollivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __COMMON_HPP__
#define __COMMON_HPP__

#include <ctime>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using std::cerr;
using std::cout;
using std::endl;
using std::map;
using std::string;
using std::vector;

typedef char              i8;
typedef short             i16;
typedef int               i32;
typedef long int          i64;
typedef long int          isize;
typedef unsigned char     u8;
typedef unsigned short    u16;
typedef unsigned int      u32;
typedef unsigned long int u64;
typedef unsigned long int usize;

string                    itoa(i64 value);
void                      _log_debug(const char* file, const char* function, int line, const std::string& message);
#define println(x) cout << "ircserver : " << x << endl
#define eprintln(x) cerr << "ircserver : " << x << endl
#define discard(x) cout << "ircserver : discarding value" << x << " in " << string(__func__) << endl
#define arraysize(x) (sizeof(x) / sizeof(x[0]))
#define todo(x) cout << "TODO : " << x << string(__func__) << endl

#define COLOR_RESET "\033[0m"
#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BLUE "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN "\033[36m"


// Logging function
#pragma once


#define log_debug(msg) _log_debug(__FILE__, __FUNCTION__, __LINE__, msg)

#if defined(__GNUC__)
#define NOEXCEPT __attribute__((nothrow))
#elif defined(_MSC_VER)
#define NOEXCEPT __declspec(nothrow)
#else
#define NOEXCEPT throw() // fallback to standard C++98 syntax
#endif

#define safe NOEXCEPT
#define unsafe throw(...) // Explicitly allows any exception to be thrown in C++98


void server_signal_handler(i32 signal);

#endif //__COMMON_HPP__
