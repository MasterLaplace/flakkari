/*
** EPITECH PROJECT, 2024
** Title: Flakkari
** Author: MasterLaplace
** Created: 2023-01-12
** File description:
** ResourceManager
*/

#include "ResourceManager.hpp"

namespace Flakkari {

std::shared_ptr<ResourceManager> ResourceManager::_instance = nullptr;
std::mutex ResourceManager::_mutex;

std::shared_ptr<ResourceManager> ResourceManager::getInstance()
{
    std::lock_guard<std::mutex> lock(_mutex);
    if (_instance == nullptr)
        _instance = std::make_shared<ResourceManager>();
    return _instance;
}

void ResourceManager::addScene(std::shared_ptr<nlohmann::json> config, const std::string &scene)
{
    std::lock_guard<std::mutex> lock(_mutex);
    if (_instance == nullptr)
        _instance = std::make_shared<ResourceManager>();
    _instance->loadConfig(config, scene);
}

void ResourceManager::deleteScene(const std::string &game, const std::string &scene)
{
    std::lock_guard<std::mutex> lock(_mutex);
    if (_instance == nullptr)
        return;
    _instance->_templates[game].erase(scene);
}

std::optional<ResourceManager::nl_template> ResourceManager::getTemplateById (
    const std::string &game, const std::string &scene, const std::string &templateId
) {
    std::lock_guard<std::mutex> lock(_mutex);
    if (_instance == nullptr)
        return std::nullopt;
    return _instance->_templates[game][scene][templateId];
}

void ResourceManager::loadConfig(std::shared_ptr<nlohmann::json> config, const std::string &scene)
{
    for (auto &_scene : (*config)["scenes"].items()) {
        for (auto sceneInfo : _scene.value().items()) {
            if (sceneInfo.key() != scene)
                continue;

            for (auto &template_ : sceneInfo.value()["templates"].items()) {
                for (auto &templateInfo : template_.value().items()) {
                    _templates[(*config)["title"]][sceneInfo.key()][templateInfo.key()] = templateInfo.value();
                }
            }
        }
    }
}

} // namespace Engine::Resource
