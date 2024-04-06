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
* 

## What's Next

This is where all of you come in to play. We are going to be addressing the following items:

### Goals

Everyone will select one of these and begin working on them, and then we will bring everyone's code back together into a singular, working simulation.

- [ ] Make the bounding box configurable via JSON
- [ ] Make ant's capacity configurable via JSON
- [ ] Give the ants a speed of 5.0 in a random direction, rather than some random speed in any direction
- [ ] Ants have a speed of 3.0 when carrying any amount of food
- [ ] Ants move in a small wave-like pattern instead of straight lines
- [ ] Create multiple clusters of food, configurable via JSON
- [ ] Delete food particles that are consumed entirely
- [ ] Send the ants home when they are full on food to deposit it
- [ ] Create an `AntHill` class that ants can deposit their food
    - [ ] All initial ants spawn from the ant hill
    - [ ] Randomly placed on map
    - [ ] Spawns new ants when food is deposited: 1 ant for every 10 food
    - [ ] Python binding code and visualization updates needed 

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
