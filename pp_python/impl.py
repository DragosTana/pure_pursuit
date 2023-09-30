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
        self.x:float = x
        self.y:float = y

    def is_closer(self, closer, farther) -> bool:
        return dist_squared(self, farther) > dist_squared(self, closer)

# non posso mettere type hint a point dentro la classe point
# quindi, ahime', eccoce, fa un po' schifo
# ma tanto ste sono usate solo per is_closer()
def dist_squared(p1:Point, p2:Point) -> float:
    dx = p1.x - p2.x
    dy = p1.y - p2.y
    return (dx**2) + (dy ** 2)

def dist(p1:Point, p2:Point) -> float:
    return math.sqrt(dist_squared(p1, p2))


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
        self.look_ahead = 1.0

        # per debugging
        # questo codice è di prova quindi la logica di debug per ora la lascio nel flusso
        # normale di esecuzione, cambia pure sta flag, non aggiunge troppa roba
        self.debug_in_visualization = True
        self.straight_intersects = []
        self.segment_intersects = []

        self.default_point = Point(-10000,-10000)

    def set_position(self, pos:Point):
        self.pos = pos

    def reset_debugging_info(self):
        self.straight_intersects = []
        self.segment_intersects = []

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

           self.reset_debugging_info()
           if s.point_in_rect(sol1):
               solutions.append(sol1)
               self.segment_intersects.append(sol1)
           else:
               self.straight_intersects.append(sol1)

           if s.point_in_rect(sol2):
               solutions.append(sol2)
               self.segment_intersects.append(sol2)
           else:
               self.straight_intersects.append(sol2)
       return solutions

    def visualize_line_intersects(self, seg:Segment, solutions:list[Point]):
        plt.plot([seg.start.x, seg.end.x], [seg.start.y, seg.end.y], '--', color='grey')
        draw_circle(self.pos.x, self.pos.y, self.look_ahead, 'orange')
    
        if len(solutions) > 0:
            for sol in solutions:
                print('solution found at [{}, {}]'.format(sol.x, sol.y))
                plt.plot([sol.x], [sol.y], '.', markersize=10, color='blue', label='sol')
            plt.legend()
        else:
            print('no intersection found!')
    
        plt.axis('scaled')
        plt.show()

    def valid_point(self, p:Point) -> bool:
        return (p is not self.default_point)
    
    def goal_path_search(self, path:list[Point]):
        # dati normali
        self.last_found_index = 10 # viene modificato &Co. quindi damo a self
        path = path

        # dati del problema vedere se esce come nell'articolo
        # self.pos = Point(1, 2.2)
        # self.look_ahead = 0.6
        # self.last_found_index = 4

        while self.last_found_index <= len(path):
            next_point = self.next_point_in(path)
            self.visualize_path_intersection(path, next_point)

            # per andare avanti (aggiunto alla cazzo canis per vedere come si evolve)
            self.pos = Point((self.pos.x + next_point.x) / 2,
                             (self.pos.y + next_point.y) / 2)
            print(self.pos.x, " : ", self.pos.y, " -- ", next_point.x, next_point.y)
            print("last found : ", self.last_found_index)
            self.reset_debugging_info()
            
    def next_point_in(self, path)->Point:
        starting_index = self.last_found_index
        # starting_index viene utilizzato solo per definire il range, così possiamo 
        # modificare last_found_index nel loop senza problemi
        # problemi che avremmo potuto avere se il range era definito con last_found_index

        for i in range(starting_index, len(path)):
            seg = Segment(path[i], path[i + 1])
            p = self.next_in_segment(seg)
            if self.valid_point(p): # se intersezione col segmento trovata
                if seg.end.is_closer(self.pos, p):
                    print(starting_index, " : " ,
                          i, " : ",
                          self.last_found_index, " : segment is failing")
                    # check per vedere se il punto trovato in questo segmento ci porta
                    # effettivamente avanti, se siamo più vicini alla fine del segmento
                    # di quanto lo sia p, allora andare da p sarebbe indetreggiare
                    # quindi andiamo avanti a cercare qualcosa che non sia p

                    # per assicurarci di andare avanti, visto che path[last_found_index]
                    # è utilizzato come fallback, qui si setta anche last_found_index in
                    # modo che il fallback funzioni, e in modo anche che poi si vada avanti
                    # dopo che il fallback ha fatto il suo corso
                    self.last_found_index = i+1
                    continue
                else:
                    # abbiamo trovato il punto, siamo nel segmento tra path[i] e path[i + 1]
                    # per restare coerenti con la definizione di last_found_index si setta
                    print(starting_index, " : ", i, " : got a good segment")
                    self.last_found_index = i
                    # e, avendo trovato il punto
                    return p
            else:
                # intersectionFound = False
                # questo è codice di fallback
                # non ho capito il codice dell'articolo, provo a incrementare
                # self.last_found_index qui e prego che torni, visto che
                # path[self.last_found_index] comunque sta indietro per invariate
                # poi boh
                self.last_found_index += 1
                print(starting_index, " : ",
                      self.last_found_index, "falling back to corner")
                return path[self.last_found_index]

        print(starting_index, " : ", self.last_found_index,
              """
              : I don't know how, I don't know why,
              yesterday you told \"evado il canone rai\"
              and all that I can see
              is just a \"stronzo porcoddi'\"
              """)
        return self.default_point

    def next_in_segment(self, seg:Segment)->Point:
        intersects = self.line_intersects(seg)
        if len(intersects) == 2:
            if seg.end.is_closer(intersects[0], intersects[1]):
                return intersects[0]
            else:
                return intersects[1]
        elif len(intersects) == 1:
            return intersects[0]
        else:
            return self.default_point

    def visualize_path_intersection(self, path, goal):
        global path_arr # abbastanza del cazzo come scelta, lo so

        field = plt.figure()
        # graph parameters
        xScale, yScale = (1, 1)
        xMin, yMin, xMax, yMax = (-1, -1, 5, 3)

        path_ax = field.add_axes([0,0,xScale,yScale])

        travelled_path = path_arr[0:self.last_found_index + 1]
        remaining_path = path_arr[self.last_found_index:]
        add_complicated_line(travelled_path, '--', 'brown', 'travelled')
        add_complicated_line(remaining_path, '--', 'grey', 'remaining')
        highlight_points(travelled_path[0:self.last_found_index], 'brown')
        highlight_points(travelled_path[self.last_found_index:], 'grey')

        # plot field
        path_ax.plot([xMin,xMax],[yMin,yMin],color='black')
        path_ax.plot([xMin,xMin],[yMin,yMax],color='black')
        path_ax.plot([xMax,xMax],[yMin,yMax],color='black')
        path_ax.plot([xMax,xMin],[yMax,yMax],color='black')
        
        # set grid
        xTicks = np.arange(xMin, xMax+1, 2)
        yTicks = np.arange(yMin, yMax+1, 2)
        
        path_ax.set_xticks(xTicks)
        path_ax.set_yticks(yTicks)
        path_ax.grid(True)
        
        path_ax.set_xlim(xMin-0.25,xMax+0.25)
        path_ax.set_ylim(yMin-0.25,yMax+0.25)
        
        # plot start and end
        path_ax.plot(path[0].x,  path[0].y, '.',color='blue',markersize=15,label='start')
        path_ax.plot(path[-1].x, path[-1].y,'.',color='green',markersize=15,label='end')

        draw_circle(self.pos.x, self.pos.y, self.look_ahead, 'orange')
        plt.plot(self.pos.x, self.pos.y, '.', markersize=15, color='orange', label='current position')

        plt.plot(goal.x, goal.y, '.', markersize=15, color='red', label='goal point')
        add_complicated_line([[self.pos.x, self.pos.y], [goal.x, goal.y]], '-', 'black', 'look ahead distance')

        if self.debug_in_visualization:
            for s in self.straight_intersects:
                plt.plot(s.x, s.y, '.', markersize=15, color='black', label='line')
    
            for s in self.segment_intersects:
                plt.plot(s.x, s.y, '.', markersize=15, color='green', label='segment')

        path_ax.legend()
        plt.show()

path_arr = [[0.0, 0.0], [0.011580143395790051, 0.6570165243709267], [0.07307496243411533, 1.2724369146199181], [0.3136756819515748, 1.7385910188236868], [0.8813313906933087, 1.9320292911046681], [1.6153051608455251, 1.9849785681091774], [2.391094224224885, 1.9878393390954208], [3.12721333474683, 1.938831731115573], [3.685011039017028, 1.7396821576569221], [3.9068092597113266, 1.275245079016133], [3.9102406525571713, 0.7136897450501469], [3.68346383786099, 0.2590283720040381], [3.1181273273535957, 0.06751996250999465], [2.3832776875784316, 0.013841087641154892], [1.5971423891000605, 0.0023698980178599423], [0.7995795475309813, 0.0003490964043320208], [0, 0]]

path_points = [Point(arr[0], arr[1]) for arr in path_arr]


r = Robot(pos=Point(0,1))
r.look_ahead = 0.6
r.goal_path_search(path_points)




