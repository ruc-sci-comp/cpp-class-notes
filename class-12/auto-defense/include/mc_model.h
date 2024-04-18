#pragma once

#include <map>
#include <vector>

#include "model.h"
#include "projectile.h"

struct MonteCarloModel
{
    auto execute_full_set(const int mcs) -> void;

    auto execute_model(const int mc) -> void;

    auto clear() -> void;

    std::map<int, std::vector<Projectile>> mc_data{};
};
