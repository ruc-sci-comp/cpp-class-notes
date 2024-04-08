# ants_v2

## Class-10 Recap

Starting with the simulation code from class-09, we spent class-10 turning it into a simulation of ants foraging. Let's recap what was done to get to this point:

* Added the `Food` class
    * We randomly spawn a cluster of food somewhere on the map
* Updated the `Entity` (ant) class to forage from food it is close too
    * Entities search the vector of food for the closest food
    * If close enough to the food, ants will forage from it
* Updated the Python binding code to know how to translate `Food` data
* Updated the visualization to display food

## What's New

Some changes have been applied to the code:

* Renamed the `Entity` class to `Ant`
* Removed log function from `Ant`
* Moved all random number generators and distributions to the top of `Model::update`

## What's Next

This is where all of you come in to play. We are going to be addressing the following items:

### Goals

Everyone will select some of these and begin working on them, and then we will bring everyone's code back together into a singular, working simulation.

#### Ant Hill

- [ ] Create an `AntHill` class that ants can deposit their food
    - [ ] Has a `position` member for setting its position on the map
    - [ ] Has a `quantity` member for tracking how much food it has stored
    - [ ] Has a function `deposit` that adds some integer to its `quantity`
    - [ ] Has a function `spawn_ants` that returns true if an ant should be spawned
        - [ ] 1 ant for every 100 food consumed
        - [ ] Spawns an ant once every second
    - [ ] Starts with enough food to spawn the specified number of ants
    - [ ] `Ant::update` is updated to also accept an `AntHill` reference
        - [ ] Ants will return to the  `AntHill's` position when full
        - [ ] Ants will deposit their food into the `AntHill` when they arrive at it
    - [ ] Add a `AntHill` member variable `anthill` to the class
    - [ ] Give the ant hill a random position within bounds
    - [ ] All ants spawn from the ant hill's position
    - [ ] Python binding code updates needed 


#### Model

- [ ] Add a `std::mt19937` member variable `eng` to the class
- [ ] Add a `std::uniform_real_distribution<>` member variable `theta_dis` to the class
- [ ] Add a `std::uniform_real_distribution<>` member variable `radius_dis` to the class
- [ ] Update `initialize`
    - [ ] Explicitly set `time` to `0.0`
    - [ ] Make the bounding box (environment limits) configurable via JSON
        - [ ] Use configuration key `"environment_bounds"`
    - [ ] Give the ants a *random direction*, rather than some random speed
    - [ ] Create multiple clusters of food, configurable via JSON
        - [ ] Use configuration keys `"number_of_food_piles"` and `"number_of_food_per_pile"`
- [ ] Update `update`:
    - [ ] Erase from the food vector all food particles that are consumed 
- [ ] Update `finalize`
    - [ ] Clear the `ants` vector
    - [ ] Clear the `food` vector

#### Ant

- [ ] Add a function `is_full` that checks if the ant is full (i.e. quantity == capacity)
- [ ] When updating an ant, it should first check if it is full, and if so it should head home
- [ ] When updating an ant's position, use its current direction and a fixed speed to determine velocity
    - [ ] Replace the `velocity` member variable with a single `double` named `direction`
    - [ ] Ants move with a speed of 5.0 when carrying no food, and move with a speed of 3.0 when carrying any amount of food
- [ ] Ants move imperfectly instead of straight lines
    - [ ] Use a normal distribution with a mean of 0.0 and std deviation of 0.1
    - [ ] The random engine and distribution can be decalred as *static* in the `update` function.
- [ ] Send the ants home when they are full on food
- [ ] When arriving at home to deposit food, reverse the direction of the ant

#### Environment

- [ ] Update restriction/reflecting in a single method
    - [ ] Instead of snapping to the boundary, move it an equal distance inbounds
    - [ ] Instead of flipping velocities (which we no longer track) we need to rotate the ant's direction to mimic a reflection
        - [ ] When hitting one of the vertical walls (left, right) we subtract the current direction from $\pi$
        - [ ] When hitting one of the horizontal walls (top, bottom) we subtract the current direction from $2\pi$
    - [ ] Update `Ant::update` accordingly

### Goals that are Sadly Too Lofty

The goal was to also simulate pheremones. **But this is tremendously difficult**. Our early decision to use free-form movement (any direction) means that dropping and tracking pheremones is exceedingly difficult, and requires a large amount of code to change or correct. I took a stab at approximately where the pheremones are dropped by simply rounding to the nearest integer coorinates; this allows for much more efficient organization and lookup of nearby pheremones. The downside is that these approximations make it very difficult to correct guide the ants a lot of the time.

The model is extremely fragile, and tuning things like field of view, evaporation rates, walking speed, and more could have huge effects on the behavior of the ants.

