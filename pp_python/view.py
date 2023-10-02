import pandas as pd
import numpy as np

def get_waypoints():
    way_points = pd.read_csv('pp_python/waypoints.csv')
    #print(way_points)
    wp_list = way_points.values.tolist()
    return wp_list