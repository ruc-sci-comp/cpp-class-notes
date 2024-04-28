#include <algorithm>

#include "utility.h"
#include "weapons_system.h"

void WeaponsSystem::engage(const Environment& environment, Projectile target)
{
    if (timer <= 0.0 && std::hypot(target.position[0] - position[0], target.position[1] - position[1]) <= engage_range)
    {
        // estimate where the target will be by the time our interceptor reaches it. 
        auto dist_to_target = compute_distance(target.position, position);
        auto time_to_reach = dist_to_target / exit_speed;

        target.update(time_to_reach, environment);

        auto launch_angle = compute_angle(target.position, position);
        interceptors.emplace_back(
            position,
            std::array<double, 2>{ exit_speed* std::cos(launch_angle), exit_speed* std::sin(launch_angle) },
            5.0,
            0.01
        );

        timer = cooldown;
    }
}

void WeaponsSystem::update(const double delta_time, const Environment& environment, std::vector<Projectile>& projectiles)
{
    for (auto& cannonshot : interceptors)
    {
        auto explode = [&](Projectile& projectile) -> bool
            {
                auto distance = compute_distance(projectile.position, cannonshot.position);
                bool destroyed = distance <= blast_radius;
                projectile.time_alive += destroyed * interceptor_fuse;
                return destroyed;
            };
        cannonshot.update(delta_time, environment); // as prescribed by the prof

        std::erase_if(projectiles, explode); // destroy projectiles if they explode
    }

    // destroy interceptors if they run out of time
    // oh cause they explode anyways even if they don't hit anything.
    std::erase_if(interceptors, [&](Projectile& interceptor) { return interceptor.time_alive >= interceptor_fuse;});

    timer -= delta_time;
}
