import math
from typing import Any
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from IPython import display
import view

from geometry import Point, Segment

class Robot:
    def __init__(self, pos:Point, look_ahead:float):
        self.pos = pos
        self.look_ahead = look_ahead
        self.lenght = 1.5 

        # per debugging
        # questo codice è di prova quindi la logica di debug per ora la lascio nel flusso
        # normale di esecuzione, cambia pure sta flag, non aggiunge troppa roba
        self.debug_in_visualization = False
        self.straight_intersects = []
        self.segment_intersects = []
        self.default_point = Point(-10000,-10000)
        self.last_found_index = 0

    def set_position(self, pos:Point):
        self.pos = pos

    def reset_debugging_info(self):
        self.straight_intersects = []
        self.segment_intersects = []

    def line_intersects(self, s:Segment) -> list[Point]:
        """
        Function that returns the intersection between the segment and the circle.
        """
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
            sol_x1 = (D * dy + np.sign(dy) * dx * np.sqrt(discriminant)) / dr**2
            sol_x2 = (D * dy - np.sign(dy) * dx * np.sqrt(discriminant)) / dr**2
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

    def valid_point(self, p:Point) -> bool:
        return (p is not self.default_point)
    
    def goal_path_search(self, path:list[Point]):
        # dati normali
        self.last_found_index = 0 # viene modificato &Co. quindi damo a self
        path = path

        while self.last_found_index <= len(path):
            next_point = self.next_point_in(path)
            self.visualize_path_intersection(path, next_point)

            # per andare avanti (aggiunto alla cazzo canis per vedere come si evolve)
            self.pos = Point((self.pos.x + next_point.x) / 2, (self.pos.y + next_point.y) / 2)
            print(self.pos.x, " : ", self.pos.y, " -- ", next_point.x, next_point.y)
            print("last found : ", self.last_found_index)
            self.reset_debugging_info()
            
    def next_point_in(self, path:list[Point]) -> Point:
        starting_index = self.last_found_index
        # starting_index viene utilizzato solo per definire il range, così possiamo 
        # modificare last_found_index nel loop senza problemi
        # problemi che avremmo potuto avere se il range era definito con last_found_index

        # python momento, def in def
        # questo è solo un +1 che tiene conto del fatto che path[len(path)] è out of bounds
        def next(i):
            return (i+1)%len(path)

        for i in range(starting_index, len(path)):
            seg = Segment(path[i], path[next(i)])
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
                    self.last_found_index = next(i)
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
                self.last_found_index = next(self.last_found_index)
                print(starting_index, " : ", self.last_found_index, "falling back to corner")
                return path[self.last_found_index]
        
        print(starting_index, " : ", self.last_found_index,
              """
                : I don't know how, I don't know why,
                yesterday you told \"evado il canone rai\"
                and all that I can see
                is just a \"stronzo porcoddi'\"
                """)
        return self.default_point
    
    def stearing_angle(self, position:Point, goal:Point, yaw_angle:float = None) -> float:
        #! NOTE: this doesn't make any fucking sense. we need real yaw angle
        alpha = math.atan2(goal.y - position.y, goal.x - position.x) #- yaw_angle
        stearing_angle = math.atan2(2.0 * self.lenght * math.sin(alpha), self.look_ahead)
        return stearing_angle
        
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

if __name__ == "__main__":

    path_arr = view.get_waypoints()
    traj = [Point(arr[0], arr[1]) for arr in path_arr]
    plot = view.ControlView(traj[0], path_arr, show_old_pos=True)
    #set start position randomly near the first point
    start_pos = traj[0] + Point(np.random.uniform(-1, 1), np.random.uniform(-1, 1))
    car = Robot(pos = start_pos, look_ahead = 6)
    stearing_angle = []
    old_pos = start_pos
    for i in range(70):
        goal = car.next_point_in(traj)
        stearing_angle.append(car.stearing_angle(car.pos, goal))
        plot.refresh(start_pos, car.pos, goal, car.look_ahead)
        l = np.random.uniform(0.4, 0.6)
        car.set_position(car.pos + (goal - car.pos) * l)
        
    
