#pragma once

#include <array>
#include <random>

#include <nlohmann/json.hpp>

#include "projectile.h"

struct Environment;
struct RadarSystem
{
    void update(const double delta_time, const Environment& environment, Projectile signal);

    std::array<double, 2> position{ 0.0, 0.0 };
    double detection_range = 500.0;

    double alpha = 1.5;
    double beta = 0.5;

    bool tracking = false;

    Projectile track_object{};

    std::mt19937_64 eng{ 1337 };
    std::normal_distribution<double> noise_distribution{ 0.0, 30.0 };
};