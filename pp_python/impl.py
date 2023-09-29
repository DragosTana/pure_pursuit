import math
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from IPython import display

# roba per il disegno
def add_line (path) : 
    for i in range (0,len(path)):
        plt.plot(path[i][0],path[i][1],'.',color='red',markersize=10)
    
    for i in range(0,len(path)-1):
        plt.plot([path[i][0],path[i+1][0]],[path[i][1],path[i+1][1]],color='b')
        
        plt.axis('scaled')
        # plt.show()

def add_complicated_line (path,lineStyle,lineColor,lineLabel) :
    for i in range (0,len(path)):
        plt.plot(path[i][0],path[i][1],'.',color='red',markersize=10)
        
    for i in range(0,len(path)-1):
        if(i == 0):
            plt.plot([path[i][0],path[i+1][0]],[path[i][1],path[i+1][1]],lineStyle,color=lineColor,label=lineLabel)
        else:
            plt.plot([path[i][0],path[i+1][0]],[path[i][1],path[i+1][1]],lineStyle,color=lineColor)        
            
    plt.axis('scaled')
            
def highlight_points (points, pointColor):
    for point in points :
        plt.plot(point[0], point[1], '.', color = pointColor, markersize = 10)
        
def draw_circle (x, y, r, circleColor):
    xs = []
    ys = []
    angles = np.arange(0, 2.2*np.pi, 0.2)        
    
    for angle in angles :
        xs.append(r*np.cos(angle) + x)
        ys.append(r*np.sin(angle) + y)
        
    plt.plot(xs, ys, '-', color = circleColor)


# roba intersezione cerchio/linea
def sign(n):
    if n >= 0:
        return 1
    return -1

class Point:
    def __init__(self, x, y):
        self.x = x
        self.y = y

class Segment:
    def __init__(self, start:Point, end:Point):
        self.start = start
        self.end = end

        self.max_x = max(start.x, end.x)
        self.max_y = max(start.y, end.y)
        self.min_x = min(start.x, end.x)
        self.min_y = min(start.y, end.y)

    def point_in_rect(self, p:Point) -> bool:
        """
        returns whether the point p is in the bounding box of the segment
        """
        return (self.min_x <= p.x <= self.max_x and
                self.min_y <= p.y <= self.max_y)

class Robot:
    def __init__(self, pos:Point):
        self.pos = pos
        self.look_ahead = 0.9

    def give_path(self, path):
        self.last_found_index = 0
        self.path = path

    def line_intersects(self, s:Segment) -> list[Point]:
       x1_off = s.start.x - self.pos.x
       y1_off = s.start.y - self.pos.y
       x2_off = s.end.x - self.pos.x
       y2_off = s.end.y - self.pos.y

       dx = x2_off - x1_off
       dy = y2_off - y1_off

       dr = math.sqrt(dx**2 + dy**2)
       D = (x1_off*y2_off) - (x2_off*y1_off)
       discriminant = (self.look_ahead**2) * (dr**2) - D**2

       solutions = []
       if discriminant >= 0:
           # calculate the solutions
           sol_x1 = (D * dy + sign(dy) * dx * np.sqrt(discriminant)) / dr**2
           sol_x2 = (D * dy - sign(dy) * dx * np.sqrt(discriminant)) / dr**2
           sol_y1 = (- D * dx + abs(dy) * np.sqrt(discriminant)) / dr**2
           sol_y2 = (- D * dx - abs(dy) * np.sqrt(discriminant)) / dr**2    

           sol1 = Point(x=sol_x1 + self.pos.x, y=sol_y1 + self.pos.y)
           sol2 = Point(x=sol_x2 + self.pos.x, y=sol_y2 + self.pos.y)

           if s.point_in_rect(sol1):
               solutions.append(sol1)

           if s.point_in_rect(sol2):
               solutions.append(sol2)

       self.visualize_intersects(s, solutions)
       return solutions
    def visualize_intersects(self, seg:Segment, sols:list[Point]):
        plt.plot([seg.start.x, seg.end.x], [seg.start.y, seg.end.y], '--', color='grey')
        draw_circle(self.pos.x, self.pos.y, self.look_ahead, 'orange')
    
        if len(sols) > 0:
            for sol in sols:
                print('solution found at [{}, {}]'.format(sol.x, sol.y))
                plt.plot([sol.x], [sol.y], '.', markersize=10, color='blue', label='sol')
            plt.legend()
        else:
            print('no intersection found!')
    
        plt.axis('scaled')
        plt.show()

    
   
    def goal_path_search(self):
        while(self.not_done_boh()):
            point = self.get_next_point()
            diocane()

r = Robot(pos=Point(0,1))
r.line_intersects(Segment(Point(2,3), Point(-2,4)))

