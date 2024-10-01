/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pollivie <pollivie.student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 15:27:31 by pollivie          #+#    #+#             */
/*   Updated: 2024/09/28 15:27:31 by pollivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

#include <cstdlib>
#include <iostream>

void announce() {

	std::cout << "\t\t" << BLUE << "+--------------------------------------------------+" << RESET << std::endl;
	std::cout << "\t\t" << BLUE << "|~5555^.....~5555555555Y?!^.........^?YPGGGGP5J7^..|" << RESET << std::endl;
	std::cout << "\t\t" << BLUE << "|!###B^.....7###BBGGBBB###B?......!PB###BBGBB###BY:|" << RESET << std::endl;
	std::cout << "\t\t" << BLUE << "|~#BBB^.....7#BBG^::::^?BBB#!....Y##BBY!^:::^!YG5!.|" << RESET << std::endl;
	std::cout << "\t\t" << BLUE << "|~BBBB^.....7#BBG: ... ^BBB#7...?#BBB!.........:...|" << RESET << std::endl;
	std::cout << "\t\t" << BLUE << "|~#BBB^.....7#BBB77777JP###G^...P#B#5..............|" << RESET << std::endl;
	std::cout << "\t\t" << BLUE << "|~#BBB^.....7#BBB#######BPJ^....5#B#P:.............|" << RESET << std::endl;
	std::cout << "\t\t" << BLUE << "|~#BBB^.....7#BBB?775#BBB?......~B#B#5~......:!J~..|" << RESET << std::endl;
	std::cout << "\t\t" << BLUE << "|~#BBB^.....7#BBG:...7B#B#P~.....^5B##BPYJJJ5G###Y:|" << RESET << std::endl;
	std::cout << "\t\t" << BLUE << "|!###B^.....7###B:....^5###BJ:.....~JPB######BG57^.|" << RESET << std::endl;
	std::cout << "\t\t" << BLUE << "|:~~~~:.....:!~~~:......~~~~!^........^~!!!!~^:....|" << RESET << std::endl;
	std::cout << "\t\t" << BLUE << "+--------------------------------------------------+" << RESET << std::endl;
}

int main(int argc, char **argv) {

	DISCARD(argv);
	if (argc != 3) {
		Logger::logError("ircser : wrong numbers of arguments provided!");
		Logger::logInfo("usage  : >$ ./ircserv <port> <password>");
		return (1);
	}
	Logger::logInfo("server : is closing now!");
	return (0);
}
