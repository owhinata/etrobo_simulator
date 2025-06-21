# etrobo_simulator

ROS2 + Gazebo simulation environment for ET Robocon.

## Requirements
- Ubuntu 22.04
- ROS2 Humble
- Gazebo 11.10.2

## Build
```bash
source /opt/ros/humble/setup.bash
colcon build --symlink-install
```

## Launch simulation
The launch file starts Gazebo with `etrobo_world` and spawns the Spike robot model. After sourcing the setup scripts, the package automatically configures `GAZEBO_MODEL_PATH` and `GAZEBO_RESOURCE_PATH` so Gazebo can find the models.

```bash
source /usr/share/gazebo/setup.bash
source install/setup.bash
ros2 launch etrobo_simulator empty_world.launch.py
```

You can adjust the spawn position using `x_pose` and `y_pose` launch arguments:
```bash
source /usr/share/gazebo/setup.bash
source install/setup.bash
ros2 launch etrobo_simulator empty_world.launch.py x_pose:=1.0 y_pose:=2.0
```
