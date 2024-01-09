/*
** EPITECH PROJECT, 2024
** Title: Flakkari
** Author: MasterLaplace
** Created: 2023-01-07
** File description:
** GameManager
*/

#include "GameManager.hpp"

namespace Flakkari {

std::shared_ptr<GameManager> GameManager::_instance = nullptr;

GameManager::GameManager()
{
}

std::shared_ptr<GameManager> GameManager::getInstance()
{
    if (!_instance)
        _instance = std::make_shared<GameManager>();
    return _instance;
}

} /* namespace Flakkari */
