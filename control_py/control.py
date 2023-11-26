"""
This module contains a Python implementation of the acutal control algorithm used on the car.
It implements Point, PIDController and PurePursuit classes.
"""
import math
import numpy as np

class Point:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def distance(self, other):
        return math.sqrt((self.x - other.x)**2 + (self.y - other.y)**2)

    def dot(self, other):
        return self.x * other.x + self.y * other.y

    def __add__(self, other):
        return Point(self.x + other.x, self.y + other.y)

    def __sub__(self, other):
        return Point(self.x - other.x, self.y - other.y)

    def __mul__(self, scalar):
        return Point(self.x * scalar, self.y * scalar)
    
    def __truediv__(self, scalar):
        return Point(self.x / scalar, self.y / scalar)

class PIDController:
    def __init__(self, kp, ki, kd, min_output, max_output):
        self.kp = kp
        self.ki = ki
        self.kd = kd
        self.min_output = min_output
        self.max_output = max_output
        self.integral = 0
        self.previous_error = 0
        self.antiwindup = False

    def calculate_vel(self, setpoint, current_value):
        error = setpoint - current_value
        output = 0.0
        derivative = error - self.previous_error

        if not self.antiwindup:
            self.integral += error
            output = self.kp * error + self.ki * self.integral + self.kd * derivative
        else:
            output = self.kp * error + self.kd * derivative
            self.antiwindup = False

        clamped_output = self.clamping(output)
        self.antiwindup = self.antiwindup_check(output, clamped_output, error)
        self.previous_error = error

        # Print debug information
        self.debug_info(error, clamped_output, output)

        return clamped_output

    def clamping(self, output):
        if output > self.max_output:
            output = self.max_output
        elif output < self.min_output:
            output = self.min_output
        return output

    def antiwindup_check(self, output, clamped_output, error):
        return (clamped_output != output) and (error * output > 0)

    def debug_info(self, error, clamped_output, output):
        # Print debug information
        print("======================================")
        print(f"error: {error}")
        print(f"antiwindup: {self.antiwindup}")
        print(f"output: {output}")
        print(f"clamped_output: {clamped_output}")

class PurePursuit:
    def __init__(self, horizon, wheel_base):
        self.horizon = horizon
        self.wheel_base = wheel_base
        self.last_closest_index = 0
        self.fractional_lookahead_index = 0.0
        self.last_goal_point = Point(0.0, 0.0)

    def closest_point(self, position, path):
        distances = [position.distance(p) for p in path[self.last_closest_index:]]
        index_of_min_distance = np.argmin(distances)
        self.last_closest_index += index_of_min_distance
        return self.last_closest_index

    def goal_point(self, position, path):
        for i in range(self.last_closest_index, len(path) - 1):
            segment_start = path[i]
            segment_end = path[i + 1]

            d = segment_end - segment_start
            f = segment_start - position

            a = d.dot(d)
            b = 2 * f.dot(d)
            c = f.dot(f) - self.horizon * self.horizon

            discriminant = b * b - 4 * a * c

            if discriminant >= 0:
                discriminant = math.sqrt(discriminant)

                # Calculate t values
                t1 = (-b - discriminant) / (2 * a)
                t2 = (-b + discriminant) / (2 * a)

                if 0 <= t1 <= 1:
                    intersection_point = segment_start + d * t1
                    if i + t1 > self.fractional_lookahead_index:
                        self.fractional_lookahead_index = i + t1
                        self.last_goal_point = intersection_point
                        return intersection_point

                if 0 <= t2 <= 1:
                    intersection_point = segment_start + d * t2
                    if i + t2 > self.fractional_lookahead_index:
                        self.fractional_lookahead_index = i + t2
                        self.last_goal_point = intersection_point
                        return intersection_point

        # If no valid lookahead point is found, use the last lookahead point
        return path[self.last_closest_index]

    def calculate_stearing(self, position, goal_point, yaw):
        alpha = math.atan2(goal_point.y - position.y, goal_point.x - position.x) - yaw
        teta = math.atan2(2.0 * self.wheel_base * math.sin(alpha), self.horizon)
        return teta

class Control(PIDController, PurePursuit):
    def __init__(self, horizon, wheel_base, kp, ki, kd, min_output, max_output):
        PIDController.__init__(self, kp, ki, kd, min_output, max_output)
        PurePursuit.__init__(self, horizon, wheel_base)

    def combined_control(self, position, velocity, yaw, path, reference_velocity):
        
        goal_point = self.goal_point(position, path)
        steering_angle = self.calculate_stearing(position, goal_point, yaw)
        velocity = self.calculate_vel(velocity, reference_velocity)

        return steering_angle, velocity