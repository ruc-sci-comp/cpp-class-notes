#include <cmath>
#include <algorithm>
#include "radar_system.h"
#include "utility.h"


void RadarSystem::update(const double delta_time, const Environment& environment, Projectile signal)
{
    const auto distance = compute_distance(signal.position, position);

    auto noise_x = (noise_distribution(eng) * distance) / detection_range;
    auto noise_y = (noise_distribution(eng) * distance) / detection_range;

    signal.position[0] += noise_x;
    signal.position[1] += noise_y;

    if (!tracking && distance <= detection_range) {
        auto dice_roll_to_start_tracking = std::uniform_real_distribution<double>{ 0.0, 1.0 };
        if (dice_roll_to_start_tracking(eng) > distance / (2 * detection_range)) {
            tracking = true;
            track_object = signal;
        }

    }
    else if (distance <= detection_range) {
        track_object.update(delta_time, environment);
        const auto r = compute_difference(signal.position, track_object.position);
        track_object.position[0] += r[0] * alpha;
        track_object.position[1] += r[1] * alpha;
        track_object.velocity[0] += r[0] * beta / delta_time;
        track_object.velocity[1] += r[1] * beta / delta_time;
    }
}
