# N-Body Problem

The [N-body problem](https://en.wikipedia.org/wiki/N-body_problem) can be described as:

    In physics, the n-body problem is the problem of predicting the individual motions of a group of celestial objects interacting with each other gravitationally.

Computationally the n-body problem has a complexity of $O(n^{2})$, since in order for us to calculate the acceleration of each body within the system we need to compute the force of gravity between each combination of bodies. For a small number of bodies the computational requirement remains low, but as the number of bodies increases, the difficulty to compute quickly becomes apparent. For a 1000-body system we need to calculate ~1000000 forces *per step* of the simulation. This means that for a 100 second simulation, ticking at 0.1 second intervals, we need 1000000000 forces computed. This does not include the calculations needed for velocity and position, and everything else that may be involved.

Regardless of the complexity, we are going to implement this system, and then also implement "optimizations" using multithreading.

## Core Algorithm

The system itself is very simple. We will have `Entity` objects that have position, velocity, acceleration, and mass. We will implement the following (indentation  some loop):

```text
1) initialize set of entities
2) while running
    3) for each entity e1 in our set
        4) for each entity e2 in our set
            5) compute the force e2 has on e1
    6) for each entity in our set
        7) apply the computed force to the entity
```

Note that the loops indicated by #3 and #4 are the source of the $O(n^{2})$ complexity! Also note the separation of computing forces and *applying* them! Computing the force of gravity $F_{G}$ from one entity onto another requires a very simple equation:

$$F_{G} = G \frac{m_{1} m_{2}}{d^{2}}$$

where:

- $G$ is the gravitational constant $6.67430e^{-11} \frac{Nm^{2}}{kg^{2}}$
- $m_{1}$ is the mass of the first body ($kg$)
- $m_{2}$ is the mass of the second body ($kg$)
- $d$ is the distance between the two bodies ($m$)

It is important to note a few things about this equation:

- This equation indicates that the gravitational force between two objects is inversely proportional to the **square** of the distance between them. This means that as objects become farther apart, the force between them quadratically decreases. Objects far apart still impart gravity on one another, but only barely.
- The gravitational constant is *small*. This means that unless objects are incredibly massive, or are incredibly close together, then the force between them will be tremendously small.
    - A pair of kilogram masses separated by a meter will experience a gravitational force of $6.67430e^{-11}N$; if you magnified that force 1 trillion times, it would be be roughly equivalent to 15 pounds of force.

Let's implement this algorithm!

## Multithreading

Our algorithms works extremely well for a decent number of bodies, but at a certain point our computers simply cannot crunch the numbers very well (and we care about real-time visualization!). This is because we are running a single thread! Let's dive in and try to multithread this algorithm!
