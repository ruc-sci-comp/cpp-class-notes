#pragma once

struct Entity;

struct Environment
{
    auto get_modification(const Entity& entity) const->std::array<double, 2>;
};
