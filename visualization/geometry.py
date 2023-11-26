import math
import numpy as np

class Point:
    def __init__(self, x, y):
        self.x:float = x
        self.y:float = y

    def is_closer(self, closer, farther) -> bool:
        return dist_squared(self, farther) > dist_squared(self, closer)

    def __add__(self, other):
        return Point(self.x + other.x, self.y + other.y)
    
    def __sub__(self, other):
        return Point(self.x - other.x, self.y - other.y)
    
    def __mul__(self, other):
        return Point(self.x * other, self.y * other)
    
    def __call__(self):
        return [self.x, self.y]

# non posso mettere type hint a point dentro la classe point
# quindi, ahime', eccoce, fa un po' schifo
# ma tanto ste sono usate solo per is_closer()

def dist_squared(p1:Point, p2:Point) -> float:
    dx, dy = (p1.x - p2.x), (p1.y - p2.y)
    return np.linalg.norm([dx, dy])

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