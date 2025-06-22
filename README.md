# etrobo_simulator

ROS2 + Gazebo simulation environment for ET Robocon.

## Requirements
- Ubuntu 22.04
- ROS2 Humble
- Gazebo 11.10.2

## Build
```bash
source /opt/ros/humble/setup.bash
rosdep install --from-paths src --ignore-src -r -y
colcon build --symlink-install
source install/setup.bash
```

## Launch simulation
The `etrobo_world.launch.py` file loads `worlds/etrobo_world.world` and spawns the Spike robot model. Environment hooks automatically configure `GAZEBO_MODEL_PATH` and `GAZEBO_RESOURCE_PATH`.

```bash
source /usr/share/gazebo/setup.bash
ros2 launch etrobo_simulator etrobo_world.launch.py
```

You can adjust the spawn position using `x_pose` and `y_pose` launch arguments:
```bash
ros2 launch etrobo_simulator etrobo_world.launch.py x_pose:=1.0 y_pose:=2.0
```

### Launch L Course

Start the L course simulation with the following command:

```bash
ros2 launch etrobo_simulator etrobo_world.launch.py x:=-0.4 y:=1.6 Y:=-1.570796327
```

## Test
```bash
colcon test --packages-select etrobo_simulator
```

