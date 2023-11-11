import matplotlib.pyplot as plt
from matplotlib.widgets import Button
from matplotlib.widgets import Slider
from matplotlib.widgets import TextBox
import numpy as np
import pandas as pd

class ControlView():
    """
    Class for visualizing the control of the car
    """

    def __init__(self, initial_pos, way_points, show_old_pos = False):
        self.position = initial_pos
        self.time = 0.3
        self.way_points = way_points
        self.show_old_pos = show_old_pos
        self.old_pos = []
        fig = plt.figure()
        self.ax = fig.subplots()
        plt.subplots_adjust(left = 0.05, right=0.8)
        xMin, yMin, xMax, yMax = (np.min([p[0] for p in way_points]), np.min([p[1] for p in way_points]),
                                  np.max([p[0] for p in way_points]), np.max([p[1] for p in way_points]))
        xMin, yMin, xMax, yMax = (xMin - 5, yMin - 5, xMax + 5, yMax + 5)
        d = max(xMax - xMin, yMax - yMin)
        plt.xlim([xMin, xMin + d])
        plt.ylim([yMin, yMin + d])
        self.ax.set_aspect('equal', adjustable='box')
        plt.plot([p[0] for p in way_points], [p[1] for p in way_points], '.', color = "red")
        plt.plot([p[0] for p in way_points], [p[1] for p in way_points],'--', color = "grey")
        
        # plot the coordinate of each waypoint
        for i, wp in enumerate(way_points):
            plt.text(wp[0] + 0.5, wp[1] + 0.5, '({}, {})'.format(trucate_float(wp[0], 1), trucate_float(wp[1], 1)), fontsize=7)
        
        axButton = plt.axes([0.85, 0.8, 0.05, 0.05])
        self.button = Button(axButton, label= "Start") 
        axSlider = plt.axes([0.85, 0.7, 0.1, 0.01])
        self.timeSlider = Slider(axSlider, 'Time', valmin= 0.01, valmax= 2, valinit= 0.01, valstep= 0.01)
        self.setTime(0)
        self.timeSlider.on_changed(self.setTime)
        self.step = 30

    def startButtonEvent(self, event):
        self.button.on_clicked(event)
    
    def setTime(self, val):
        self.time = self.timeSlider.val
        
    def draw_circle (self, x, y, r, color):
        xs = []
        ys = []
        angles = np.arange(0, 2.2*np.pi, 0.2)        

        for angle in angles :
            xs.append(r*np.cos(angle) + x)
            ys.append(r*np.sin(angle) + y)

        return [xs, ys]      
    
    def refresh(self, startPoint, position, goal_point, horizon):
        """
        Refresh the plot with the new position of the car
        """

        if startPoint != None:
            start_x, start_y = startPoint()
            plotStart = self.ax.plot(start_x, start_y, 'o',markersize = 10 , color = "black")[0]
        
        if self.show_old_pos:
            position = position[-10:]
            plot_old_pos = self.ax.plot(position.x, position.y, '.',markersize = 10 , color = "blue")[0]
        
        plot_pos = self.ax.plot(position.x, position.y, 'o',markersize = 10 , color = "green")[0] 
        plot_goal = self.ax.plot(goal_point.x, goal_point.y, 'o',markersize = 10 , color = "red")[0]
        xs, ys = self.draw_circle(position.x, position.y, horizon, "green")
        plot_circ = self.ax.plot(xs, ys, color = "green")[0]
        
        plt.pause(self.time)
        
        plot_pos.remove()        
        plot_goal.remove()
        plot_circ.remove()

plt.show()

def trucate_float(f, n):
    s = '{}'.format(f)
    if 'e' in s or 'E' in s:
        return '{0:.{1}f}'.format(f, n)
    i, p, d = s.partition('.')
    return '.'.join([i, (d+'0'*n)[:n]])

def get_waypoints():
    way_points = pd.read_csv('waypoints.csv')
    #print(way_points)
    wp_list = way_points.values.tolist()
    return wp_list

if __name__ == "__main__":
    
    traj = get_waypoints()
    view = ControlView(traj[0], traj)
    plt.show()
