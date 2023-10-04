import view
import impl

def main():
    
    path_arr = view.get_waypoints()
    traj = [impl.Point(arr[0], arr[1]) for arr in path_arr]
    plot = view.ControlView(traj[0], path_arr, show_old_pos=True)

    while True:
        message = input()
        pos_x, pos_y, goal_x, goal_y, horizon = message.split(":")
        pos = impl.Point(float(pos_x), float(pos_y))
        goal = impl.Point(float(goal_x), float(goal_y))
        horizon = float(horizon)
        
        plot.refresh(pos, goal, horizon)