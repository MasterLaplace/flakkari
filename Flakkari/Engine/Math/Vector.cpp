/*
** EPITECH PROJECT, 2023
** Title: Flakkari
** Author: MasterLaplace
** Created: 2024-01-05
** File description:
** Vector
*/

#include "Vector.hpp"

namespace Flakkari::Engine::Math {

template <typename Type> std::ostream &operator<<(std::ostream &os, const Vector<Type> &vector)
{
    os << "Vector(x: " << vector.x << ", y: " << vector.y << ", z: " << vector.z << ", w: " << vector.w << ")";
    return os;
}

} // namespace Flakkari::Engine::Math
