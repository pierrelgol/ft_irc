/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pollivie <pollivie.student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 09:37:31 by pollivie          #+#    #+#             */
/*   Updated: 2024/09/29 09:37:31 by pollivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include "Colors.hpp"

#include <cstdio>
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/time.h>

enum LogLevel {
        DEBUG,
        INFO,
        SUCCESS,
        WARN,
        ERROR,
};

static const char *getLevelString(LogLevel level) {
        switch (level) {
                case DEBUG: return "D";
                case INFO: return "I";
                case SUCCESS: return "S";
                case WARN: return "W";
                case ERROR: return "E";
        }
        return "";
}

static const char *getColor(LogLevel level) {
        switch (level) {
                case DEBUG: return YELLOW;
                case INFO: return WHITE;
                case SUCCESS: return GREEN;
                case WARN: return MAGENTA;
                case ERROR: return RED;
        }
        return RESET;
}

static std::string getCurrentTime() {
        char           buffer[32];
        struct timeval tv;
        gettimeofday(&tv, NULL);
        struct tm *ptm = localtime(&tv.tv_sec);
        std::sprintf(buffer, "%02d:%02d:%02d", ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
        return std::string(buffer);
}

class Logger {
      public:
        static void log(LogLevel level, const std::string &message) {
                std::cout << getColor(level) << "[" << getLevelString(level) << "]" << "[" << getCurrentTime() << "] " << message << RESET << std::endl;
        }

        template <typename T>
        static void logDebug(const T &message) {
                logStream(DEBUG, message);
        }

        template <typename T>
        static void logInfo(const T &message) {
                logStream(INFO, message);
        }

        template <typename T>
        static void logSuccess(const T &message) {
                logStream(SUCCESS, message);
        }

        template <typename T>
        static void logWarn(const T &message) {
                logStream(WARN, message);
        }

        template <typename T>
        static void logError(const T &message) {
                logStream(ERROR, message);
        }

        template <typename T>
        static void logStream(LogLevel level, const T &message) {
                std::ostringstream oss;
                oss << message;
                log(level, oss.str());
        }
};

#endif // LOGGER_HPP
