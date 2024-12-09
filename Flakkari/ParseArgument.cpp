/*
** EPITECH PROJECT, 2024
** Title: Flakkari
** Author: MasterLaplace
** Created: 2024-12-07
** File description:
** ParseArgument
*/

#include "ParseArgument.hpp"

namespace Flakkari {

ParseArgument::ParseArgument(int ac, const char *av[])
{
    for (uint_fast8_t i = 0; i < ac; ++i)
    {
        if (std::string(av[i]) == "-d" || std::string(av[i]) == "--default")
        {
            _gameDir = "Games";
            _ip = "localhost";
            _port = 8081;
            return;
        }
        else if (std::string(av[i]) == "-gameDir")
        {
            _gameDir = av[i + 1];
            ++i;
        }
        else if (std::string(av[i]) == "-ip")
        {
            _ip = av[i + 1];
            ++i;
        }
        else if (std::string(av[i]) == "-port")
        {
            _port = static_cast<unsigned short>(std::stoi(av[i + 1]));
            ++i;
        }
        else if (std::string(av[i]) == "-h" || std::string(av[i]) == "--help")
        {
            std::cout << HELP_MESSAGE << std::endl;
            exit(0);
        }
        else if (std::string(av[i]) == "-v" || std::string(av[i]) == "--version")
        {
            std::cout << "Flakkari Library v" FLAKKARI_VERSION_STRING " - © 2024 MasterLaplace" << std::endl;
            exit(0);
        }
    }

    if (_ip.empty())
        _ip = "localhost";

    if (_port == 0)
        _port = 8081;
}

} /* namespace Flakkari */
