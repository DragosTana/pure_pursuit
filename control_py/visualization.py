import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import pandas as pd
from control import Point, PIDController, PurePursuit, Control
import car_model

# Your car model and control code here (assuming you've already defined classes and functions)

# get waypoints from csv file
waypoints = pd.read_csv('waypoints.csv')
waypoints = [Point(x, y) for x, y in zip(waypoints['x'], waypoints['y'])]

# Initialize car and control objects
car = car_model.NonLinearBicycleModel(x=waypoints[0].x, y=waypoints[0].y, yaw=0, vx=1.0, vy=0.0, omega=0.0)
controller = Control(horizon=5.0, wheel_base=car_model.L, kp=1.0, ki=0.1, kd=0.01, min_output=-1.0, max_output=1.0)

# Variables for simulation
dt_sim = 0.1
total_time = 100.0
num_steps = int(total_time / dt_sim)
current_time = 0.0

# Lists to store simulation data for plotting
car_states = []
car_yaw = []
steering_angles = []
velocities = []

# Simulation loop
for _ in range(num_steps):
    goal_point = controller.goal_point(Point(car.x, car.y), waypoints)
    steering_angle, velocity = controller.combined_control(
        velocity=car.vx,
        position=Point(car.x, car.y),
        yaw=car.yaw,  # Replace 'yaw' with the correct attribute in your NonLinearBicycleModel class
        path=waypoints,
        reference_velocity=20.0  # Set a reference velocity for the simulation
    )

    car.update(throttle=1.0, delta=steering_angle)
    car_states.append(Point(car.x, car.y))
    car_yaw.append(car.yaw)
    steering_angles.append(steering_angle)
    velocities.append(velocity)

    current_time += dt_sim

# Visualization
fig, ax = plt.subplots()

car_body = plt.Rectangle((car.x, car.y), 4.0, 2.0, np.degrees(car.yaw), edgecolor='r', facecolor='none', label='Car')

def update(frame):
    ax.clear()
    
    # Plot the car body
    car_body.set_xy((car_states[frame].x - 2.0, car_states[frame].y - 1.0))
    car_body.angle = np.degrees(car_yaw[frame])  # Replace 'yaw' with the correct attribute in your NonLinearBicycleModel class
    ax.add_patch(car_body)

    # Plot the waypoints
    waypoint_x = [point.x for point in waypoints]
    waypoint_y = [point.y for point in waypoints]
    ax.plot(waypoint_x, waypoint_y, 'r--', label='Waypoints')
    
    # Plot the path
    path_x = [point.x for point in car_states]
    path_y = [point.y for point in car_states]
    ax.plot(path_x, path_y, 'b-', label='Path')

    ax.set_title(f'Time: {current_time:.1f}s')
    ax.set_xlabel('X-axis')
    ax.set_ylabel('Y-axis')
    ax.legend()

ani = animation.FuncAnimation(fig, update, frames=num_steps, interval=100)
plt.show()
