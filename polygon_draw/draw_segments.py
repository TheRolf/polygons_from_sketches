import os
import time
from random import uniform

import matplotlib.pyplot as plt
from matplotlib.patches import Polygon

base = os.path.dirname(os.path.realpath(__file__))
folder = "./../res/"


def write_points():
    global points
    ofile = open(os.path.join(folder, "points.txt"), 'w')
    if len(points) % 2 == 1:
        temp_x, temp_y = points[-1]
        points = points[:-1]
    else:
        temp_x, temp_y = None, None
    ofile.write(f"{int(len(points) / 2)}\n")
    for x, y in points:
        ofile.write(f"{round(x, 3)} {round(y, 3)}\n")
    if temp_x and temp_y:
        points.append([temp_x, temp_y])
    ofile.close()


def calculate_polygons():
    os.startfile(os.path.join(base, "./../polygon/x64/Release/polygon.exe"))


def draw_polygons():
    ifile = open(os.path.join(folder, "polygons.txt"), 'r')
    Npol = int(ifile.readline().strip())
    polygons = [[] for i in range(Npol)]
    for i in range(Npol):
        Nside = int(ifile.readline().strip())
        for j in range(Nside):
            line = ifile.readline().strip()
            sline = line.split(' ')
            x, y = map(float, sline)
            polygons[i].append([x, y])
    ax.patches = []

    for polygon in polygons:
        polygon = Polygon(polygon, facecolor=(uniform(0.5, 1), uniform(0.5, 1), uniform(0.5, 1)), zorder=1)
        ax.add_patch(polygon)
        # for x, y in polygon:
        #     ax.scatter(x, y, s=20, c='r', zorder=3)
    plt.draw()


def show_intersections():
    ifile = open(os.path.join(folder, "polygons_.txt"), 'r')


def delete_points():
    global points
    points = []


def read_points():
    global n_segments, points
    n_segments = 0
    ifile1 = open(os.path.join(folder, "points.txt"), 'r')
    x_old, y_old = None, None

    for line in ifile1:
        sline = line.strip().split(' ')
        if len(sline) > 1:
            if x_old and y_old:
                x, y = map(float, sline)
                points.append([x, y])
                ax.scatter(x, y, s=10, c='k', zorder=2)
                ax.plot((x, x_old), (y, y_old), 'k', zorder=2)
                ax.text((x + x_old) / 2, (y + y_old) / 2, str(n_segments))
                n_segments += 1
                x_old, y_old = None, None
            else:
                x_old, y_old = map(float, sline)
                points.append([x_old, y_old])
                ax.scatter(x_old, y_old, s=10, c='k', zorder=2)


def keypress(event):
    global n_segments
    if event.key == '0':
        write_points()
        calculate_polygons()
        time.sleep(0.5)
        draw_polygons()

    elif event.key == '1':
        write_points()

    elif event.key == '2':
        calculate_polygons()

    elif event.key == '3':
        draw_polygons()

    elif event.key == '4':
        n_segments = 0
        delete_points()
        ax.cla()
        ax.set_aspect(1)
        plt.xlim([0, 20])
        plt.ylim([0, 10])
        plt.show()
        write_points()

    elif event.key == '5':
        read_points()
        ax.cla()
        ax.set_aspect(1)
        plt.xlim([0, 20])
        plt.ylim([0, 10])
        plt.show()


def onclick(event):
    global x_old, y_old, points, n_segments
    x, y = event.xdata, event.ydata
    if x and y:
        print(x, y)
        points.append([x, y])
        ax.scatter(x, y, s=10, c='k', zorder=2)
        if x_old and y_old:
            ax.plot((x, x_old), (y, y_old), 'k', zorder=2)
            ax.text((x+x_old)/2, (y+y_old)/2, str(n_segments))
            n_segments += 1
            x_old, y_old = None, None
        else:
            x_old, y_old = x, y
    plt.draw()


n_segments = 0
points = []
x_old, y_old = None, None
fig, ax = plt.subplots(figsize=(12, 6))
ax.set_aspect(1)
plt.xlim([0, 20])
plt.ylim([0, 10])
fig.canvas.mpl_connect('button_press_event', onclick)
fig.canvas.mpl_connect('key_press_event', keypress)

plt.show()


