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
ros2 launch etrobo_simulator etrobo_world.launch.py x:=-0.4 y:=1.57 Y:=-1.570796327
```

## Test
```bash
colcon test --packages-select etrobo_simulator
```

## Tips: Gazebo/Simulation Troubleshooting

If the simulation fails to launch (e.g. `gzserver` dies immediately, `/spawn_entity` service unavailable, or robot cannot be spawned in Gazebo), please check the following:

1. **Check for leftover Gazebo processes**

   - Previous runs may leave `gzserver` or `gzclient` running in the background, which can cause failures.
   - To check and clean up any remaining processes:

   ```bash
   ps -e | grep gz
   killall gzserver gzclient
   # Or to kill individually:
   kill <PID>
   ```

2. **Re-source your ROS 2 workspace after building**

   - Always run:

   ```bash
   source install/setup.bash
   ```

3. **Check file existence**

   - Make sure all referenced world and model files exist in `worlds/` and `models/` directories.

4. **Check terminal output for error details**

   - Sometimes errors are only printed to the terminal (not saved in log files). Look for missing library, plugin, or permission errors.

5. **(Advanced) Isolate with standalone Gazebo launch**

   - Try running Gazebo directly:

   ```bash
   gazebo src/etrobo_simulator/worlds/etrobo_world.world
   ```

If problems persist after these steps, please check your ROS/Gazebo install and environment variables, or seek additional support.
