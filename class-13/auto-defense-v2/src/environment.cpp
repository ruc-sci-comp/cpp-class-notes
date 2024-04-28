#include <algorithm>
#include <cmath>
#include <numbers>

#include "environment.h"
#include "projectile.h"
#include "utility.h"

using namespace std::numbers;

// coefficient of drag of a spherical object
constexpr auto CD = double{ 0.47 };


auto Environment::get_drag(const Projectile& projectile) const -> std::array<double, 2>
{
    auto a = pi * projectile.radius * projectile.radius;
    auto p = get_air_density(projectile.position[1]);

    auto dxy = compute_difference(projectile.velocity, wind);

    return {
        -0.5 * a * p * CD * dxy[0] * dxy[0] / projectile.mass,
        -0.5 * a * p * CD * dxy[1] * dxy[1] / projectile.mass
    };
}

auto Environment::get_air_density(double alt) const -> double
{
    // need to compute the air density at the given altitude!
    alt = std::clamp(alt, altitude.front(), altitude.back());
    auto higher = [&](auto a) -> bool
        {
            return a >= alt;
        };
    auto upper_it = std::ranges::find_if(altitude, higher);
    auto upper_idx = std::distance(altitude.begin(), upper_it);
    auto lower_idx = upper_idx - 1;
    auto t = (alt - altitude[lower_idx]) / (altitude[upper_idx] - altitude[lower_idx]);
    return std::lerp(density[lower_idx], density[upper_idx], t);
}

auto Environment::get_gravity(double alt) const -> double
{
    // need to compute the gravity at the given altitude!
    alt = std::clamp(alt, altitude.front(), altitude.back());
    auto higher = [&](auto a) -> bool
        {
            return a >= alt;
        };
    auto upper_it = std::ranges::find_if(altitude, higher);
    auto upper_idx = std::distance(altitude.begin(), upper_it);
    auto lower_idx = upper_idx - 1;
    auto t = (alt - altitude[lower_idx]) / (altitude[upper_idx] - altitude[lower_idx]);
    return std::lerp(gravity[lower_idx], gravity[upper_idx], t);
}