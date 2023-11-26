import csv

from argparse import ArgumentParser
from atexit import register
from matplotlib import pyplot as plt
from click_generator import ClickGenerator

def exit_handler(x, y, z):

    with open("waypoints.csv", "w", newline='') as f:
        writer = csv.writer(f)

        if z:
            writer.writerow(['x', 'y', 'z'])
            writer.writerows(zip(x, y, z))
        else:
            writer.writerow(['x', 'y'])
            writer.writerows(zip(x, y))

def main():

    try:
        # Parameters
        map_size = 100
        line_colour = '#F0A39A'
        point_colour = '#383831'

        fig = plt.figure()
        ax = plt.axes()
        ax.set_aspect('equal', adjustable='box')
        ax.set_xlim(-map_size, map_size)
        ax.set_ylim(-map_size, map_size)
        z = None

        click_gen = ClickGenerator(ax, fig, map_size, line_colour, point_colour, scroll_sensitivity=1)
        x, y = click_gen.generate()

        plt.grid()
        plt.show()
        register(exit_handler, x, y, z)

    except KeyboardInterrupt:
        register(exit_handler, x, y, z)

if __name__ == '__main__':
    main()
