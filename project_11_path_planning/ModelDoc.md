# Model Documentation

## Task Description
The goal in this project in to **build a path planner that is able to create smooth ,safe trajectories** for the car to follow. The highway track has other vehicles, all going different speeds, but approximately obeying the 50 MPH speed limit.

The car transmits its location, along with its sensor fusion data,which estimates the location of all the vehicles on the same side of the road.

## What is the Path

A **trajectory** is constituted by as set of (x, y) points. Every 20 ms the car moves to next point on the list.
In this framework the car moves from point to point perfectly.

Since the car always moves to the next waypoint after 20ms, the **velocity** of the vehicle is given by how much future waypoints are space.
In other words, the longer the space between waypoints, the faster the car will be.

## Rebric

Here is a bullet list of rubric points along with an explanation of how constraints are satisfied.

- **The car drives according to the speed limit**
    A [reference velocity variable](https://github.com/ndrplz/self-driving-car/blob/7b0d9e057931667a04422649fee6d017b3ef7475/project_11_path_planning/src/main.cpp#L74) is used to keep track of current speed of the vehicle. Also, [another constant variable](https://github.com/ndrplz/self-driving-car/blob/7b0d9e057931667a04422649fee6d017b3ef7475/project_11_path_planning/src/utils.h#L13) is used to make sure the car never exceed the maximum speed of 50 mph.

- **Max acceleration and jerk are note exceed.**    
- **也就是最大加速度和jerk(加速度的变化率)不能超过最大变化范围才行**
    The rubric sets as hard constraint the fact that car does not exceed a total acceleration of 10m/s^2 and jerk of 10m/s^3. This is obtained by setting the initial speed of vehicle to zero, and[gradually accelerating](ttps://github.com/ndrplz/self-driving-car/blob/7b0d9e057931667a04422649fee6d017b3ef7475/project_11_path_planning/src/main.cpp#L181-L182)until the vehicle has reached the maximum speed allowed (either by the speed limit or by the current road situation).
