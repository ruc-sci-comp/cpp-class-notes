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

Some changes have been applied to the code that make it a little less abstract:

* Renamed the `Entity` class to `Ant`

## What's Next

This is where all of you come in to play. We are going to be addressing the following items:

### Goals

Everyone will select one of these and begin working on them, and then we will bring everyone's code back together into a singular, working simulation.

#### Ant Hill

- [ ] Create an `AntHill` class that ants can deposit their food
    - [ ] Has a `position` member for setting its position on the map
    - [ ] Has a `quantity` member for tracking how much food it has stored
    - [ ] Has a function `deposit` that adds some integer to its `quantity`
    - [ ] Has a function `spawn_ants` that returns a number of new ants to add to the simulation by consuming food
        - [ ] 1 ant for every 100 food consumed
    - [ ] `Ant::update` is updated to also accept an `AntHill` reference
        - [ ] Ants will return to the  `AntHill's` position when full
        - [ ] Ants will deposit their food into the `AntHill` when they arrive at it
    - [ ] Python binding code updates needed 

#### Model

- [ ] Add a `std::mt19937` member variable `eng` to the class
- [ ] Add a `std::uniform_real_distribution<>` member variable `theta_dis` to the class
- [ ] Add a `std::uniform_real_distribution<>` member variable `radius_dis` to the class
- [ ] Add a `AntHill` member variable `anthill` to the class
- [ ] Update `initialize`
    - [ ] Explicitly set `time` to `0.0`
    - [ ] Make the bounding box (environment limits) configurable via JSON
        - [ ] Use configuration key `"environment_bounds"`
    - [ ] Give the ant hill a random position within bounds
    - [ ] All ants spawn from the ant hill's position
    - [ ] Give the ants a *random direction*, rather than some random speed
    - [ ] Create multiple clusters of food, configurable via JSON
        - [ ] Use configuration keys `"number_of_food_piles"` and `"number_of_food_per_pile"`
- [ ] Update `update`:
    - [ ] Erase from the food vector all food particles that are consumed 
    - [ ] Spawn new ants from the ant hill as needed
- [ ] Update `finalize`
    - [ ] Clear the `ants` vector
    - [ ] Clear the `food` vector

#### Ant

- [ ] Add a function `is_full` that checks if the ant is full (i.e. quantity == capacity)
- [ ] When updating an ant, it should first check if it is full, and if so it should head home
- [ ] When updating an ant's position, use its current direction to and a fixed speed to determine velocity
    - [ ] Replace the `velocity` member variable with a single `double` named `direction`
    - [ ] Ants move with a speed of 5.0 when carrying no food, and move with a speed of 3.0 when carrying any amount of food
- [ ] Ants move imperfectly instead of straight lines
    - [ ] Use a normal distribution with a mean of 0.0 and std deviation of 0.1
    - [ ] The random engine and distribution can be decalred as *static* in the `update` function.
- [ ] Send the ants home when they are full on food or are not close enough to food and currently have food

### Final Goals

Once we have everything above completed we will together work through implementing the *pheremone trails* that ants leave behind:

- [ ] Add a pheremone grid to the `Environment`
- [ ] Ants drop one of the following pheremones
    - [ ] exploring (ants use this to backtrack home)
    - [ ] going home with food (ants use this to go back to food)
- [ ] Pheremones fade away over time

### Stretch Goals

- [ ] Update `Environment::reflect` to work on a "wall" at any angle
- [ ] Add internal walls to the environment, with ants unable to pass through them
    - [ ] Walls are added as line segments
    - [ ] If the delta-position of an ant would cross a wall, it is clamped to the wall and reflected
