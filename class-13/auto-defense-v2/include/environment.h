#pragma once

#include <array>
#include <vector>

struct Projectile;
struct Environment
{
    auto get_drag(const Projectile& projectile) const -> std::array<double, 2>;

    auto get_air_density(double a) const -> double;

    auto get_gravity(double a) const -> double;


    std::array<double, 2>wind{};
    std::vector<double> altitude{ -1000, 0, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000, 15000, 20000, 25000, 30000, 40000, 50000, 60000, 70000, 80000 };
    std::vector<double> gravity{ -9.810, -9.807, -9.804, -9.801, -9.797, -9.794, -9.791, -9.788, -9.785, -9.782, -9.779, -9.776, -9.761, -9.745, -9.730, -9.715, -9.684, -9.654, -9.624, -9.594, -9.564 };
    std::vector<double> density{ 1.347, 1.225, 1.112, 1.007, 0.9093, 0.8194, 0.7364, 0.6601, 0.5900, 0.5258, 0.4671, 0.4135, 0.1948, 0.08891, 0.04008, 0.01841, 0.003996, 0.001027, 0.0003097, 0.00008283, 0.00001846 };
};
