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

#include "Client.hpp"
#include "Server.hpp"

#include <iostream>

void announce() {

	std::cout << "\t\t" << IRCCOLOR << "+--------------------------------------------------+" << RESET << std::endl;
	std::cout << "\t\t" << IRCCOLOR << "|~5555^.....~5555555555Y?!^.........^?YPGGGGP5J7^..|" << RESET << std::endl;
	std::cout << "\t\t" << IRCCOLOR << "|!###B^.....7###BBGGBBB###B?......!PB###BBGBB###BY:|" << RESET << std::endl;
	std::cout << "\t\t" << IRCCOLOR << "|~#BBB^.....7#BBG^::::^?BBB#!....Y##BBY!^:::^!YG5!.|" << RESET << std::endl;
	std::cout << "\t\t" << IRCCOLOR << "|~BBBB^.....7#BBG: ... ^BBB#7...?#BBB!.........:...|" << RESET << std::endl;
	std::cout << "\t\t" << IRCCOLOR << "|~#BBB^.....7#BBB77777JP###G^...P#B#5..............|" << RESET << std::endl;
	std::cout << "\t\t" << IRCCOLOR << "|~#BBB^.....7#BBB#######BPJ^....5#B#P:.............|" << RESET << std::endl;
	std::cout << "\t\t" << IRCCOLOR << "|~#BBB^.....7#BBB?775#BBB?......~B#B#5~......:!J~..|" << RESET << std::endl;
	std::cout << "\t\t" << IRCCOLOR << "|~#BBB^.....7#BBG:...7B#B#P~.....^5B##BPYJJJ5G###Y:|" << RESET << std::endl;
	std::cout << "\t\t" << IRCCOLOR << "|!###B^.....7###B:....^5###BJ:.....~JPB######BG57^.|" << RESET << std::endl;
	std::cout << "\t\t" << IRCCOLOR << "|:~~~~:.....:!~~~:......~~~~!^........^~!!!!~^:....|" << RESET << std::endl;
	std::cout << "\t\t" << IRCCOLOR << "+--------------------------------------------------+" << RESET << std::endl;
}

int main(void) {

	Server ircserv;

	announce();

	try {
		signal(SIGINT, Server::sig_handler);
		signal(SIGQUIT, Server::sig_handler);
		ircserv.init();
		ircserv.run();
	}catch (const std::exception &e) {
		ircserv.deinit();
		Logger::logError(e.what());
	}
	Logger::logInfo("server : is closing now!");
	return (0);
}
