from matplotlib import pyplot as plt
from waypoint_generator import WaypointGenerator

class ClickGenerator(WaypointGenerator):

    def __init__(self, ax, fig, map_size, line_colour, point_colour, scroll_sensitivity):
        
        super().__init__(ax, fig, map_size, line_colour, point_colour)

        self.onpress_dict = {
            'z': self.remove_last_point,
            'x': self.remove_all_points,
            'c': self.connect_ends
        }

        self.scroll_sensitivity = scroll_sensitivity if scroll_sensitivity else 1

    def remove_last_point(self):

        self.x.pop()
        self.y.pop()

    def remove_all_points(self):

        del self.x[:]
        del self.y[:]

    def connect_ends(self):

        self.x.append(self.x[0])
        self.y.append(self.y[0])

    def generate(self):

        def update_plot():

            plt.cla()
            plt.grid()
            self.ax.plot(self.x, self.y, '-', color=self.line_colour)
            self.ax.plot(self.x, self.y, '.', color=self.point_colour)
            self.ax.set_xlim(-self.map_size, self.map_size)
            self.ax.set_ylim(-self.map_size, self.map_size)

            self.fig.canvas.draw()

        def onclick(event):
            
            if not event.xdata or not event.ydata:
                return

            self.x.append(event.xdata)
            self.y.append(event.ydata)

            update_plot()

        def onpress(event):

            func = self.onpress_dict.get(event.key)

            if not func:
                return

            func()
            update_plot()

        def onscroll(event):

            if not self.z:
                return

            elif event.button == 'up':
                self.z[-1] += self.scroll_sensitivity

            elif event.button == 'down':
                self.z[-1] -= self.scroll_sensitivity

            update_plot()

        self.fig.canvas.mpl_connect('button_press_event', onclick)
        self.fig.canvas.mpl_connect('key_press_event', onpress)

        return self.x, self.y