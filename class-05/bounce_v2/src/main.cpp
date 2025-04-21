#include <iostream>
#include <vector>  // this gives us access to the vector data type

using namespace std;

// This code simulates 5 balls falling (due to gravity) and bouncing on the floor.
// The idea is simple: use equations to update the position and velocity of each ball,
// and whenever the ball "hits" the ground, we make it bounce by flipping the velocity
// in the other direction. We also simulate the fact that when something bounces it
// does not go as high as it did before, and do we use a *coefficient of restitution*
// to dampen the velocity with each bounce.
// 
// Last week we created variables x and y to represent the position of a single entity;
// doing this for multiple entities becomes very inconvenient, as we would need to
// create multiple sets of variables for each entity, and then write code to update
// each separately. This gets insane to code when we have more than 2 or 3 entities.
//
// In order for us to simulate many entities at once, we pack their data into vectors.
// By placing the x values, y values, and velocity values in vectors, we can now use
// a loop to simulate all of the entities, rather than explicitly coding the equations
// for each entity.
// 
auto main() -> int
{
    // In order to simulate the physics of the balls falling, we need to establish time.
    // We will increment our simulation in 0.1 second intervals
    auto delta_time = double{ 0.1 };

    // We want to create balls at different coordinates. We start with two vectors, one
    // for the x and one for the y components of their coordinates. Below we want to create
    // 5 balls at the following coordinates:
    //
    // b1: (0, 60)
    // b2: (1, 70)
    // b3: (2, 80)
    // b4: (3, 90)
    // b5: (4, 100)
    //
    auto xs = vector<double>{ 0, 1, 2, 3, 4 };
    auto ys = vector<double>{ 60, 70, 80, 90, 100 };

    // Every ball is initially stationary, floating in the air. This means they
    // all have a velocity of 0!
    auto vs = vector<double>{ 0, 0, 0, 0, 0 };

    // Every ball is subjected to gravity, so we can say the acceleration of each
    // ball is -9.81 m/s/s. This is constant for all balls, so we need only a double.
    auto acceleration = double{ -9.81 };

    // This is the coefficient of restitution. We will set it to 0.8, which means
    // that after each bounce, the ball that bounced will retain 80% of its kinetic
    // energy.
    auto coeff_of_restitution = double{ 0.8 };

    // This is our main simulation loop. We will simulate the passage of time by
    // creating a variable named "time" set to 0, and we will continue simulating
    // until 50 seconds have passed. Every time we advance the simulation, it will
    // move forward by only 0.1 seconds (which is the value of delta_time) 
    for (auto time = double{}; time < 50.0; time += delta_time)
    {
        // Every time we move time forward, we need to move the entities forward through
        // time. Because all of the data is in a vector, we can readily use a for-loop to
        // apply the same equations to all entities. This is much better than writing the
        // same block of code 5 time!
        //
        // All of our vectors are the same size, so we can just use one of them to set up
        // the loop. Here i represents which ball we are currently updating.
        for (auto i = size_t{}; i < xs.size(); ++i)
        {
            // First let's pull out the data associated with the current ball we are simulating
            auto x = xs[i];
            auto y = ys[i];
            auto v = vs[i];

            // This is where things get tricky. In order for us to simulate the bounce, we need
            // to ensure that the ball does not go through the ground. Since the balls will be
            // incrementally moving, it is very possible that an increment would make its altitude
            // (the y-coordinate of position) less than 0. Therefore, we first need to calcuate
            // how far down the ball is falling - our delta y, or just dy.
            auto dy = v * delta_time;

            // We need to check if the new position (y plus dy) is at or under ground. If it then
            // we need to make sure that the collision with the ground is simulated properly. So
            // if we anticipate falling through the ground (which we cannot do!)...
            if (y + dy <= 0.0)
            {
                // We need to figure out how long it will take to hit the ground from the current
                // position. We know how high the ball is and how fast it is moving, so dividing
                // that distance by the speed will give us the time to impact (which is less than
                // 0.1 seconds!). 
                auto time_before_bounce = y / v;

                // Once we know how long it will take to hit the ground, we can subtract it from
                // the original delta time to calculate how much time to simulate after the bounce.
                // Every step of the simulation needs to ultimately be 0.1 seconds, so all we are
                // doing here is breaking that 0.1 seconds into a "before bounce" and "after bounce".
                auto time_after_bounce = delta_time - time_before_bounce;

                // With both sections of time calcuated, we can now apply Euler integration to figure
                // out how high the ball will bounce. First we need to figure out exactly how fast
                // the ball is moving when it hits the ground.
                auto velocity_at_impact = v + acceleration * time_before_bounce;

                // Once we know how fast it is moving when it hits the ground, we can apply the
                // coefficient of restitution to the velocity to determine how fast it is going
                // after the bounce. We make the impact velocity negative, so as to "turn" the
                // ball around.
                v = -velocity_at_impact * coeff_of_restitution;

                // Lastly, we can apply gravity to the ball's velocity.
                v += acceleration * time_after_bounce;

                // Now that we know how fast the ball is moving after the bounce, we can simply
                // apply Euler integration here to calculate how high the bounce is.
                y = v * time_after_bounce;
            }
            else
            {
                // If we are not bouncing, we can very easily just apply Euler integration.
                v += acceleration * delta_time;
                y += v * delta_time;
            }

            // Once we have the final y and v values, we just throw them right back into the
            // vetor, overwriting what was there previously.
            ys[i] = y;
            vs[i] = v;

            // Log the data - we log i here so that we can know what data is associated with
            // which ball.
            cout << time << " " << i << " " << x << " " << y << " " << v << endl;
        }
    }
}
