/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pollivie <pollivie.student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 09:37:31 by pollivie          #+#    #+#             */
/*   Updated: 2024/09/29 09:37:31 by pollivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOG_HPP
#define LOG_HPP

#include <iostream>
#include <ctime>
#include <string>
#include <sys/time.h>
#include <cstdio>
#include <sstream>

enum LogLevel { DEBUG, INFO, SUCCESS, WARN, ERROR };

static const char *getLevelString(LogLevel level)
{
	switch (level) {
	case DEBUG:
		return "DEBUG";
	case INFO:
		return "INFO";
	case SUCCESS:
		return "SUCCESS";
	case WARN:
		return "WARN";
	case ERROR:
		return "ERROR";
	}
	return "";
}

static const char *getColor(LogLevel level)
{
	switch (level) {
	case DEBUG:
		return "\033[33m";
	case INFO:
		return "\033[37m";
	case SUCCESS:
		return "\033[32m";
	case WARN:
		return "\033[35m";
	case ERROR:
		return "\033[31m";
	}
	return "\033[0m";
}

static std::string getCurrentTime()
{
	char buffer[16];
	struct timeval tv;
	gettimeofday(&tv, NULL);
	struct tm *ptm = localtime(&tv.tv_sec);
	std::sprintf(buffer, "%02d:%02d:%02d", ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
	return std::string(buffer);
}

class Logger {
    public:
	static void log(LogLevel level, const std::string &message)
	{
		std::cout << getColor(level) << "[" << getLevelString(level) << "]" << "[" << getCurrentTime() << "] " << message << "\033[0m" << std::endl;
	}

	template <typename T> static void logDebug(const T &message)
	{
		logStream(DEBUG, message);
	}

	template <typename T> static void logInfo(const T &message)
	{
		logStream(INFO, message);
	}

	template <typename T> static void logSuccess(const T &message)
	{
		logStream(SUCCESS, message);
	}

	template <typename T> static void logWarn(const T &message)
	{
		logStream(WARN, message);
	}

	template <typename T> static void logError(const T &message)
	{
		logStream(ERROR, message);
	}

	template <typename T> static void logStream(LogLevel level, const T &message)
	{
		std::ostringstream oss;
		oss << message;
		log(level, oss.str());
	}

};

const Logger log;

#endif // LOG_HPP
