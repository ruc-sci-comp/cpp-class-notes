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

Due to complexity, this is currently being omitted.

~~The C3 system is responsible for the integration of the other components into a central system. The C3 system will listen for radar data and eventually send commands to the weapons system to engage the cannonball. In actual applications this type of centralized system has the benefit of being able to work with data from many sources and can notify many other subsystems simultaneously (as opposed to each subsystem needing to know how to communicate with each other).~~

~~Our C3 system will be responsible for talking to the radar and weapon system, and it will determine if and when a cannonball is a risk. It will also forward radar measurements to the weapon system should action need to be taken.~~

## Weapon System

In *Ballistic Missile Defense* (BMD), the goal is to intercept a missile with one of our own missiles (also called interceptors). Less frequently in recent years, interceptors were largely *kinetic*, meaning that they are designed to physically collide with the target to knock it out of the sky. In our simulation we will attempt to knock the cannonballs down with our own cannonballs. This is extremely impractical in realitity, but simulating such a system is relatively much easier.

## Class-12 Recap & Changes

Class-12 ended with our ballistic model (mostly) completed, and a semblance of a Monte Carlo simulation constructed. Again the goal is to simulate many separate scenarios and see how well our system can intercept the cannonballs in each scenario.

Some changes since last class:

- Added empty classes `RadarSystem` and `WeaponsSystem`
- Consolidated dashboards
- Refined randomized launch velocities
- Projectiles have a `time_alive` member variable for tracking how long they have been flying, which we can use later for detonating interceptors. 

## Completing the Model

### Environment

We need to account for wind! This will affect aerodynamic drag by simply offsetting the velocities in the equations for x- and y-drag. Wind will be constant for an entire execution of the system.

- [ ] Add an array of length 2 to the environment to represent the wind vector
- [ ] Update the `get_drag` method to account for wind
- Add a configuration parameter for setting wind randomly. In the config file these should be under their own section `"environment"`:
    - [ ] Wind should blow at a random angle -10 to 10, or 170 to 190 degrees
    - [ ] Wind should blow at a speed between a lower and upper bound (0.0 to maybe 10.0?)

- Bonus Behavior
    - [ ] Wind is not always blowing
    - [ ] Wind direction and speed changes over time

### Radar System

The radar model is responsible for tracking the cannonball. We will implement an *alpha-beta* filter for providing state estimates. The radar system has one method: `update`. Here is what we need to do for the radar:

- The radar should determine if the *signal* is close enough (the signal is just a projectile). It needs to implement the logic to "see" the projectile, and then to "track" it's movement. 
    - [ ] compute the distance from the radar to the signal position
    - [ ] if the radar is not tracking and the signal is close enough, set tracking to true, and assign the signal to the track object.
    - [ ] else if the signal is close enough (and thus already tracking):
        - [ ] update the track object
        - [ ] compute `r` as the difference between the signal and track object
        - [ ] adjust the track object position by $r \alpha$
        - [ ] adjust the track object velocity by $\frac{r \beta}{\Delta T}$
    - [ ] Bonus Behavior - add uncertainly to the radar:
        - [ ] Add a random number generator and a normal distribution to the class (mean of `0.0`, stddev of `30.0`)
        - [ ] Compute `noise_x` and `noise_y` respectively for the x- and y- dimensions
             - Noise is equal to $n = rd/D$ where $r$ is a random number, $d$ is the distance to the signal, and $D$ is the detect-range of the radar
        - [ ] Add each noise to the respective position component of the signal
    - [ ] Bonus behavior - randomize the detection
        - [ ] Add a uniform distribution to the class (from 0.0 to 1.0)
        - [ ] Instead of immediately tracking when in range, generate a random number with the new distribution
        - [ ] Start tracking if the random number is greater than $d/2D$ (This means there is a 50% to detect at max range)

- Once the radar is completed, we need to add it to the `model` as a part of the simulation. The radar is already there in the class, but we need to configure it in `initialize` and call its `update` method in `update`.

- [ ] The following need to be added to the configuration file, and then needs to be used to initialize the radar. In the config file these should be under their own section `"radar"`:
    - [ ] `position`: location of the radar, set to `[1700.0, 0.0]`
    - [ ] `detection_range`: how far the radar can detect, set to `1200.0`
    - [ ] `alpha`: signal gains for estimating position, set to `0.5` 
    - [ ] `beta`: signal gains for estimating velocity, set to `0.1`
- [ ] Bonus configuration: if randomness added to the radar
    - [ ] `unertainty`: standard deviation of the normal distribution

### Weapons System

- The weapons system has two functions: `engage` and `update`. `engage` is responsible for firing on a projectile, and `update` is responsible for detonating interceptors. We need to implement both functions:

- [ ] `enage`
    - [ ] The weapons system can only engage a target once per its cooldown; if the cooldown is 0.0 (or less) then it can fire a projectile.
    - [ ] Assuming the system is ready, we need to check if the target is in range. If it is, then we can engage the target!
        - [ ] In order for us to engage, we need to estimate where the target will be by the time our interceptor reaches it. The weapon system has an exit speed, which defines how fast the interceptor is going at launch. The time to reach the target is (mostly) just the distance to the target, divided by the exist speed. With this time, we can update the target in time by the time-to-reach to estimate where it'll be.
    - [ ] Once the target is updated, we can use the estimated position to compute a launch angle, and thus a launch vector for the interceptor. We can then add a new interceptor to the system to launch.
        - [ ] Use the weapons system position, newly calculated launch vector, mass of 5.0, and radius of 0.01 for the new interceptor.

- [ ] `update`
    - The update function is responsible for updating all of the interceptors, as well as destroying any projectiles that are within range of an interceptor.
    - [ ] Use a for-loop to update all interceptors
    - [ ] Write a lambda function that takes a `Projectile` as a const reference, and captures a `&`.
        - Compute the distance between the projectile and the interceptor
        - Create a bool `destroyed` that is equal to a comparison that checks if the distance is less-than-or-equal-to the blast radius
        - Increment the `time_alive` of the interceptor by `destroyed` multiplied by the `interceptor_fuse` (this will "trigger" it)
        - Return `destroyed`
    - [ ] After updating all interceptors, use `std::erase_if` to find and delete all interceptors that have lived beyond the `interceptor_fuse`.
    - [ ] Decrement `timer` by `delta_time`

- Once the weapons system is completed, we need to add it to the `model` as a part of the simulation. The weapons system is already there in the class, but we need to configure it in `initialize` and call its `engage` and `update` methods in `update` accordingly.

- [ ] The following need to be added to the configuration file, and then needs to be used to initialize the weapons system. In the config file these should be under their own section `"weapons"`:
    - [ ] `position`: location of the radar, set to `[1700.0, 0.0]`
    - [ ] `engage_range`: how far the weapon system can engage, set to `750.0`
    - [ ] `exit_speed`: how fast interceptors are launched at, set to `400.0`
    - [ ] `interceptor_fuse`: how long interceptors live for, set to `10.0`
    - [ ] `blast_radius`: how close an interceptor needs to be to successfully intercept a target, set to `10.0`
    - [ ] `cooldown`: how frequently the weapons system can launch an interceptor, set to `0.25`

- [ ] Bonus configuration: allow the interceptors to be configured (mass, radius). 

### Other Updates

- [ ] Update `Model::initialize` to also take a constant integer argument for the `seed`; this will be used to see the main random number generator.

### Scoring

Time permitting, we can implement a `ScoreCard` class. This is a new class that would be used by `MonteCarloModel` that would provide the following details:
    - [ ] Intercepted
    - [ ] Impact distance

`MonteCarloMNodel` would have another map that maps Monte Carlo numbers to `ScoreCards`. This data would then be used to provide better success metrics. What else could we add here?
