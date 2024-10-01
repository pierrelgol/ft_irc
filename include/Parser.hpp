/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pollivie <pollivie.student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 14:16:32 by pollivie          #+#    #+#             */
/*   Updated: 2024/10/01 14:16:32 by pollivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
#define PARSER_HPP

#include "Common.hpp"

class Parser {
      public:
	static bool is_hexdigit(char c) {
		return (isdigit(c) || (c >= 'A' && c <= 'F'));
	}

	static bool is_letter(const std::string& str) {
		if (str.length() != 1) return false;
		char c = str[0];
		return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
	}

	static bool is_digit(const std::string& str) {
		if (str.length() != 1) return false;
		return isdigit(str[0]) != 0;
	}

	// Check if the string is a valid nospcrlfcl (any octet except NUL, CR, LF, ' ' and ':')
	static bool is_nospcrlfcl(const std::string& str) {
		for (size_t i = 0; i < str.length(); ++i) {
			char c = str[i];
			if (c == 0 || c == 13 || c == 10 || c == ' ' || c == ':') {
				return false;
			}
		}
		return true;
	}

	// Check if the string is a valid middle (nospcrlfcl followed by optional ":" or nospcrlfcl)
	static bool is_middle(const std::string& str) {
		if (str.empty()) return false;
		if (!is_nospcrlfcl(str.substr(0, 1))) return false;

		// All characters must be either ":" or valid nospcrlfcl
		for (size_t i = 1; i < str.length(); ++i) {
			char c = str[i];
			if (c != ':' && !is_nospcrlfcl(std::string(1, c))) {
				return false;
			}
		}
		return true;
	}

	// Check if the string is a valid trailing (":" / " " / nospcrlfcl)
	static bool is_trailing(const std::string& str) {
		for (size_t i = 0; i < str.length(); ++i) {
			char c = str[i];
			if (c != ':' && c != ' ' && !is_nospcrlfcl(std::string(1, c))) {
				return false;
			}
		}
		return true;
	}

	// Check if the string is a valid command (1*letter or 3digit)
	static bool is_command(const std::string& str) {
		// If all characters are digits and length is exactly 3, it's valid
		if (str.length() == 3 && isdigit(str[0]) && isdigit(str[1]) && isdigit(str[2])) {
			return true;
		}
		// Otherwise, must be all letters (1*letter)
		for (size_t i = 0; i < str.length(); ++i) {
			if (!is_letter(std::string(1, str[i]))) return false;
		}
		return !str.empty();
	}

	// Check if the string is a valid nickname (1 letter or special followed by up to 8 letters/digits/specials)
	static bool is_nickname(const std::string& str) {
		if (str.empty() || (!is_letter(std::string(1, str[0])) && (str[0] < '[' || str[0] > '}'))) {
			return false;
		}
		for (size_t i = 1; i < str.length() && i <= 8; ++i) {
			char c = str[i];
			if (!is_letter(std::string(1, c)) && !isdigit(c) && !(c >= '[' && c <= '}') && c != '-') {
				return false;
			}
		}
		return true;
	}

	// Check if the string is a valid SPACE (space character)
	static bool is_space(const std::string& str) {
		return str == " ";
	}

	// Check if the string is a valid crlf (carriage return + linefeed)
	static bool is_crlf(const std::string& str) {
		return str == "\r\n";
	}
};

#endif // PARSER_HPP
