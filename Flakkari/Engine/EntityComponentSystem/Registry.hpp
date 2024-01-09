/**************************************************************************
 * Flakkari Library v0.2.0
 *
 * Flakkari Library is a C++ Library for Network.
 * @file Registry.hpp
 * @brief Registry class for ECS (Entity Component System).
 *
 * Flakkari Library is under MIT License.
 * https://opensource.org/licenses/MIT
 * © 2023 @MasterLaplace
 * @version 0.2.0
 * @date 2023-01-05
 **************************************************************************/


#ifndef REGISTRY_HPP_
    #define REGISTRY_HPP_

#include "SparseArrays.hpp"
#include "Entity.hpp"

#include <unordered_map>
#include <typeindex>
#include <any>
#include <functional>
#include <queue>
#include <stdexcept>
#include <climits>
#include <iostream>

namespace Flakkari::Engine::ECS {

class Registry {
public:
    using entity_type = Entity;
    using EraseFn = std::function<void(Registry&, const entity_type&)>;
    using SystemFn = std::function<void(Registry&)>;

    /**
     * @brief Spawn a new entity in the registry.
     *
     * @return entity_type  The entity created.
     */
    entity_type spawn_entity();

    /**
     * @brief Get the entity from index object from the registry.
     *
     * @param idx  The index of the entity.
     * @return entity_type  The entity.
     */
    entity_type entity_from_index(std::size_t idx);

    /**
     * @brief Kill an entity from the registry.
     *
     * @param e  The entity to kill.
     */
    void kill_entity(const entity_type &e);

    /**
     * @brief Check if an entity is registered in the registry.
     *
     * @tparam Component  The component to check.
     * @param entity  The entity to check.
     * @return true  If the entity is registered.
     * @return false  If the entity is not registered.
     */
    template <typename Component>
    [[nodiscard]] bool isRegistered(entity_type const &entity)
    {
        auto componentIt = _components.find(std::type_index(typeid(Component)));

        if (componentIt != _components.end()) {
            auto &component = std::any_cast<SparseArrays<Component>&>(componentIt->second);
            if (entity < component.size())
                return component[entity].has_value();
        }
        return false;
    }

    /**
     * @brief Check if a component is registered in the registry.
     *
     * @tparam Component  The component to check.
     * @return true  If the component is registered.
     * @return false  If the component is not registered.
     */
    template <typename Component>
    [[nodiscard]] bool isRegistered()
    {
        auto componentIt = _components.find(std::type_index(typeid(Component)));

        if (componentIt != _components.end()) {
            auto &component = std::any_cast<SparseArrays<Component>&>(componentIt->second);
            return component.size() > 0;
        }
        return false;
    }

    /**
     * @brief Get the component from an entity.
     *
     * @tparam Component  The component to get.
     * @param to  The entity to get the component from.
     * @param c  The component to get.
     * @return SparseArrays<Component>::reference_type  The component.
     */
    template <typename Component>
    typename SparseArrays<Component>::reference_type add_component(const entity_type &to, Component &&c) {
        return getComponents<Component>().insert_at(to, std::forward<Component>(c));
    }

    /**
     * @brief Get the component from an entity.
     *
     * @tparam Component  The component to get.
     * @tparam Params  The parameters to construct the component.
     * @param to  The entity to get the component from.
     * @param p  The parameters to construct the component.
     * @return SparseArrays<Component>::reference_type  The component.
     */
    template <typename Component, typename... Params>
    typename SparseArrays<Component>::reference_type emplace_component(const entity_type &to, Params&&... p) {
        return getComponents<Component>().emplace_at(to, std::forward<Params>(p)...);
    }

    /**
     * @brief Remove a component from an entity in the registry.
     *
     * @tparam Component  The component to remove.
     * @param from  The entity to remove the component from.
     */
    template <typename Component>
    void remove_component(const entity_type &from) {
        getComponents<Component>().erase(from);
    }

    /**
     * @brief Get the component from an entity.
     *
     * @tparam Component  The component to get.
     * @return SparseArrays<Component>&  The component.
     */
    template <typename Component>
    SparseArrays<Component> &registerComponent()
    {
        if (isRegistered<Component>())
            return getComponents<Component>();

        auto ti = std::type_index(typeid(Component));
        _components[ti] = std::make_any<SparseArrays<Component>>();
        _eraseFunctions[ti] =  [](Registry &r, const entity_type &e) {
            r.remove_component<Component>(e);
        };
        return std::any_cast<SparseArrays<Component>&>(_components[ti]);
    }

    /**
     * @brief Get the Components object from the registry.
     *
     * @tparam Component  The component to get.
     * @return SparseArrays<Component>&  The component array.
     */
    template <typename Component>
    SparseArrays<Component> &getComponents()
    {
        if (!isRegistered<Component>())
            return registerComponent<Component>();

        auto ti = std::type_index(typeid(Component));
        return std::any_cast<SparseArrays<Component>&>(_components[ti]);
    }

    /**
     * @brief Get the Components object from the registry.
     *
     * @tparam Component  The component to get.
     * @return const SparseArrays<Component>&  The component array.
     */
    template <typename Component>
    const SparseArrays<Component> &getComponents() const {
        auto ti = std::type_index(typeid(Component));
        return std::any_cast<const SparseArrays<Component>&>(_components.at(ti));
    }

    /**
     * @brief Add a system to the registry.
     *
     * @tparam Components  The components to add to the system.
     * @tparam Function  The function to add to the system.
     * @param f  The function to add to the system.
     */
    template <typename... Components, typename Function>
    void add_system(Function &&f) {
        _systems.emplace_back([sys{std::forward<Function>(f)}](Registry &r) {
            sys(r, r.getComponents<Components>()...);
        });
    }

    /**
     * @brief  Run all the systems in the registry.
     *
     */
    void run_systems();

private:
    std::unordered_map<std::type_index, std::any> _components;
    std::unordered_map<std::type_index, EraseFn> _eraseFunctions;
    std::vector<SystemFn> _systems;
    std::queue<entity_type> _deadEntities;
    size_t _nextEntity = 0;
};

} // namespace Flakkari::Engine::ECS

#endif /* !REGISTRY_HPP_ */
