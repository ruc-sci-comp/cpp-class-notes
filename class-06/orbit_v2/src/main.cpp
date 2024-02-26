#include <cmath>
#include <vector>
#include <fmt/format.h>


// Forward declaration
auto integrate(
    double& position,
    double& velocity,
    const double acceleration,
    const double time_delta) -> void;


// We want to simulate simple orbits of the planets within our solar system! This is a
// grossly oversimplified simulation though!
auto main() -> int
{
    // the Gravitational constant
    auto gravitational_constant = double{ 6.674e-11 };

    // tick time forward 1 week at a time
    auto time_delta = double{ 604800.0 };

    // needed to compute accelerations due to gravity; we will ignore interplanetary gravity for this
    auto sun_mass = double{ 1.989e30 };

    // helper variable
    auto number_of_planets = size_t{ 8 };

    // basic data taken from https://nssdc.gsfc.nasa.gov/planetary/factsheet/
    // only capture data through neptune
    // pluto's orbit is simply too slow and large to meaningfully represent in 10 years of simulation time
    auto planets_x = std::vector<double>{ 57.9e9, 108.2e9, 149.6e9,  228.0e9, 778.5e9, 1432.0e9, 2867.0e9, 4515.0e9 };
    auto planets_y = std::vector<double>(number_of_planets, 0.0);
    auto planets_vx = std::vector<double>(number_of_planets, 0.0);
    auto planets_vy = std::vector<double>{ 47.4e3, 35.0e3, 29.8e3, 24.1e3, 13.1e3, 9.7e3, 6.8e3, 5.4e3 };

    // print initial states (week 0)
    for (auto i = size_t{}; i < number_of_planets; ++i)
    {
        fmt::print("{:.4f} {} {:.4f} {:.4f} {:.4f} {:.4f}\n", 0.0, i, planets_x[i], planets_y[i], planets_vx[i], planets_vy[i]);
    }

    // start iterating with week 1
    for (auto time = double{ 604800.0 }; time < 3.154e8; time += time_delta)
    {
        // for every planet, calculate the force of gravity imparted by the sun using:
        // F = G * m1 * m2 / (d * d)
        //
        // F: force due to gravity
        // G: the Gravitational constant
        // m1: mass of entity 1
        // m2: mass of entity 2
        // d: distance between the two entities
        //
        // however, since we also know that F = ma, we can reduce our equation above to
        // compute acceleration:
        //
        // a = G * m2 / (d * d)
        //
        // where m2 is the mass of the imparting entity (the sun)
        for (auto i = size_t{}; i < number_of_planets; ++i)
        {
            // compute distance between the planet and the sun
            auto distance = std::hypot(planets_x[i], planets_y[i]);

            // compute the angle relative to the positive x-axis between the planet and the sun
            auto angle = std::atan2(-planets_y[i], -planets_x[i]);

            // compute the gravitational acceleration vector
            auto acceleration = gravitational_constant * sun_mass / (distance * distance);

            // use trigonometry to break the acceleration vector into x and y components
            auto acceleration_x = acceleration * std::cos(angle);
            auto acceleration_y = acceleration * std::sin(angle);

            // use Backwards Euler Integration to integrate the planet
            integrate(planets_x[i], planets_vx[i], acceleration_x, time_delta);
            integrate(planets_y[i], planets_vy[i], acceleration_y, time_delta);

            // log the planets data
            fmt::print("{:.4f} {} {:.4f} {:.4f} {:.4f} {:.4f}\n", time, i, planets_x[i], planets_y[i], planets_vx[i], planets_vy[i]);
        }
    }
}

// integrate is a function that applies Backwards Euler Integration to the given
// position, velocity, and acceleration.
//
// position and velocity are passed as references, allowing the function to update
// their values directly.
auto integrate(
    double& position,
    double& velocity,
    const double acceleration,
    const double time_delta) -> void
{
    velocity += acceleration * time_delta;
    position += velocity * time_delta;
}
