#include <algorithm>
#include <cmath>
#include <numbers>

#include "environment.h"
#include "projectile.h"

using namespace std::numbers;

// coefficient of drag of a spherical object
constexpr auto CD = double{ 0.47 };

auto Environment::get_drag(const Projectile&) const -> std::array<double, 2>
{
    // need to compute drag!
    return { 0.0, 0.0 };
}

auto Environment::get_air_density(double) const -> double
{
    // need to compute the air density at the given altitude!
    return 0.0;
}

auto Environment::get_gravity(double) const -> double
{
    // need to compute the gravity at the given altitude!
    return -9.81;
}