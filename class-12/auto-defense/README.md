# auto-defense

The goal of this project is to create a simulation of an automated ballistic defense system, with its performance measured using Monte Carlo techniques. This is directly representative of work conducted at defense companies like Lockheed Martin for the Aegis Combat System.

## Simulation Overview

Our model will revisit our cannonball simulation from early this semester, however we will add a number of complexities to it, namely aerodynamic drag and wind. We will randomize things like its launch velocity, mass, aerodynamic coefficients, and more in an attempt to land the cannonball within a target area.

Once the cannonball is sufficiently modeled and is working as a Monte Carlo simulation, we will begin developing the defense system, which will consist of a radar system, C3 system (command/control/communication), and a weapons system.

## Monte Carlo Simulation

The idea is simple - we will simulate some model that contains randomness for one or more components, and we will run that model many times. As we run the model over and over *using a different seed for the random number generator(s)* the average behavior of the model should converge on some result.

Our model will look at few different metrics, but for starters we will only care whether or not the cannonball hits the ground within the target area. Eventually, time permitting, we will also look at detection rate, track rate,and false engagement rates.

## Radar System

It is far too complex to implement a real radar, and so instead we will use a state estimator called an *Alpha-Beta-Gamma Filter*. This is a simple technique for estimating the state of some object given estimations of its traversal through time.

Our radar model will get measurements of the cannonballs by applying some random noise to the actual position of the cannonball. The farther away the cannonball is, the more noise will we will apply to the measurements. The radar will need to *detect* the cannonball before it begins *tracking* it; to do this we will also randomize the detection of an object based on distance. Once the radar begins tracking and as the object becomes closer to the radar, it should eventually begin predicting the position of the cannonball *and its velocity* extremely well.

The radar will initially be designed to have a full 360 degree view, but if time permitting we will tune this down to a more conventially radar beam.

## Command, Control, & Communication

The C3 system is responsible for the integration of the other components into a central system. The C3 system will listen for radar data and eventually send commands to the weapons system to engage the cannonball. In actual applications this type of centralized system has the benefit of being able to work with data from many sources and can notify many other subsystems simultaneously (as opposed to each subsystem needing to know how to communicate with each other).

Our C3 system will be responsible for talking to the radar and weapon system, and it will determine if and when a cannonball is a risk. It will also forward radar measurements to the weapon system should action need to be taken.

## Weapon System

In *Ballistic Missile Defense* (BMD), the goal is to intercept a missile with one of our own missiles (also called interceptors). Less frequently in recent years, interceptors were largely *kinetic*, meaning that they are designed to physically collide with the target to knock it out of the sky. In our simulation we will attempt to knock the cannonballs down with our own cannonballs. This is extremely impractical in realitity, but simulating such a system is relatively much easier.

## Getting Started

We begin with a simple cannonball model that needs a little bit of updating. Once those updates are complete, we will transition it to a Monte Carlo model where a single execution of the model results in *many* trajectories.

Let's first just run the model and see what it does. Pretty simple!

We need to do the following:

- [ ] Implement the functions in [`src/environment.cpp`](src/environment.cpp):
    - [ ] `get_drag` should compute the aerodynamic drag of the given projectile.
        - The formula for drag is found [here](https://en.wikipedia.org/wiki/Drag_equation)
    - [ ] `get_air_density` and `get_gravity` are linear interplating functions. The `Environment` class contains data (taken from [here](https://www.engineeringtoolbox.com/standard-atmosphere-d_604.html)) that has various altitudes and measurements of air density and gravity at those altitudes.
        - These functions should find the altitudes in the data around it to create lower and upper bounds. The positions of the surrounding altitudes can be used to look up the surrounding density or gravity values. From the given altitude and lower/upper bounds of that value we can compute the fractional value `t`, which is the relative % between the bounds. We can use this value `t` to linearly interpolate the density and gravity values as needed.

        $$t = \frac{v - l}{u - l}$$

        where $t$ is the fractional value, $v$ is the altitude we are intepolating to, and respectively $l$ and $u$ are the lower and upper altitudes bounding our desired altitude.

        We can then use `std::lerp` from the `cmath` header to perform the actual density and gravity interpolation. How can we improve this code?
- [ ] Update `Model::initialize` and our configuration file with the following:
    - [ ] `Model::initialize` should have an optional argument `seed` that is used to set the seed if it is not present in the configuration file.
        - We can use the function `eng.seed` to override the seed. 
    - [ ] Configure a random wind vector
        - This will also require changes to the `Environment` and affect our drag calculations  
    - [ ] Configure multiple random projectiles (random launch angle, launch speed). Launch vectors should still be fairly focused and not totally random!
- [ ] Update `Projectile` to maintain a unique ID so that we can tell them apart (requries changes to `model.cpp` to set them)
- [ ] Update `model.cpp` to "freeze" projectiles once they impact the ground, and in doing so also mark the simulation as "complete" when all projectiles have impacted.

These changes should give us sufficient randomness in our trajectories.

## Monte Carlo Model

Now we need to provide the mechanisms for running our simulations MANY times! This requires us to wrap our model in a new class `MonteCarloModel` that will be responsible for creating `Model` objects and running them to completion. We need to do the following:

- [ ] Let's create new files `mc_model.h` and `mc_model.cpp` so that we can begin defining the class `MonteCarloModel`
    - [ ] Our class is fairly simple, it needs to define the following:
        - [ ] `execute_full_set` - a function that takes in a number of iterations of the model to execute, and then uses a for-loop to call `execute_model` repeatedly
        - [ ] `execute_model` - a function that create, initializes, runs, and finalizes a model with the given Monte Carlo number (which we can use for the seed!)
        - [ ] `clear` - clears the `mc_data` data map
        - [ ] `mc_data` - a `std::map` that associates MC numbers to projectile data. This acts as a HUGE data buffer in memory!

Once we implement this class, we can run a new dashboard to visualize a large number of randomized trajectory data!
