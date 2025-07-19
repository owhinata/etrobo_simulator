#!/usr/bin/env python3

import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import ExecuteProcess
from launch.substitutions import LaunchConfiguration


def generate_launch_description():
    
    # Get the urdf file
    ETROBO_MODEL_FILE = os.path.join(get_package_share_directory('etrobo_simulator'), 'models', 'plastic_bottle', 'model.sdf')
    
    # Launch configuration variables specific to simulation
    x = LaunchConfiguration('x', default='-0.4')
    y = LaunchConfiguration('y', default='-0.5')
    z = LaunchConfiguration('z', default='0.0')
    Y = LaunchConfiguration('Y', default='0.0')

    # Spawn Entity
    spawn_entity = ExecuteProcess(
        cmd=['ros2', 'service', 'call', '/spawn_entity', 'gazebo_msgs/SpawnEntity',
             '{name: "plastic_bottle", xml: "", initial_pose: {position: {x: ', x, ', y: ', y, ', z: ', z, '}, orientation: {x: 0.0, y: 0.0, z: 0.0, w: 1.0}}, reference_frame: "world"}'],
        output='screen'
    )

    # Actually spawn with SDF file
    spawn_entity_sdf = ExecuteProcess(
        cmd=['ros2', 'run', 'gazebo_ros', 'spawn_entity.py', 
             '-entity', 'plastic_bottle_obstacle',
             '-file', ETROBO_MODEL_FILE,
             '-x', x, '-y', y, '-z', z, '-Y', Y],
        output='screen'
    )

    # Create the launch description and populate
    ld = LaunchDescription()
    ld.add_action(spawn_entity_sdf)

    return ld