import pandas as pd
import numpy as np


class ControlViewer:
    def __init__ (self):
        pass
    
    def refresh(self, position, waypoint, goal, look_ahead):
        self.position = position
        self.waypoint = waypoint
        self.goal = goal
        self.look_ahead = look_ahead
        pass        
        

def get_waypoints():
    way_points = pd.read_csv('pp_python/waypoints.csv')
    #print(way_points)
    wp_list = way_points.values.tolist()
    return wp_list