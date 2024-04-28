#pragma once

#include <array>
#include <vector>
#include <nlohmann/json.hpp>

#include "projectile.h"

struct Environment;

struct WeaponsSystem
{
    void engage(const Environment& environment, Projectile target);

    void update(double dt, const Environment& environment, std::vector<Projectile>& projectiles);

    std::array<double, 2> position{ 0.0, 0.0 };

    double engage_range = 250.0;
    double exit_speed = 250.0;
    double interceptor_fuse = 3.0;
    double blast_radius = 5.0;
    double cooldown = 1.0;
    double timer = 0.0;

    std::vector<Projectile> interceptors = {};
};
