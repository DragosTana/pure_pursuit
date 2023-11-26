# Control Node
This repository contains alle the logic for the FR-24 autonomus car control system.

## PID

The PID controls are repsonible for the longitudinal control of the car. The implementation is pretty standard, anti-windup techinques and low pass filter on derivative are implemented.

To compile, from the root directory use:

``` 
cd pid
g++ main.cpp -o pid
```
To visualize, from the root directory use:

``` 
cd visualization
python3 pid_visual.py
```

## Pure Pursuit

Pure Pursuit algorithm is responsible for the lateral control of the vehicle. Nothing fancy here for now!

To compile, from the root directory use:

``` 
cd pure_pursuit
g++ main.cpp PurePursuit.cpp -o pp
```

To visualize, from the root directory use:

``` 
cd visualization
./pure_pursuit/pp <horizion> | python3 visualization/pp_visual.py
```
