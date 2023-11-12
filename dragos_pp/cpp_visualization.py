import view
from geometry import Point
def main():
    
    path_arr = view.get_waypoints()
    traj = [Point(arr[0], arr[1]) for arr in path_arr]
    plot = view.ControlView(traj[0], path_arr, show_old_pos=True)

    while True:
        message = input()
        #print(message)
        pos_x, pos_y, goal_x, goal_y, horizon = message.split(":")
        pos = Point(float(pos_x), float(pos_y))
        goal = Point(float(goal_x), float(goal_y))
        horizon = float(horizon)
        
        plot.refresh(traj[0], pos, goal, horizon)
        
main()
