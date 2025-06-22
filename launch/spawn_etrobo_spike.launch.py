# Copyright 2019 Open Source Robotics Foundation, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import os

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():
    # Get the urdf file
    model_folder = 'etrobo_spike'
    urdf_path = os.path.join(
        get_package_share_directory('etrobo_simulator'),
        'models',
        model_folder,
        'model.sdf'
    )

    # Launch configuration variables specific to simulation
    x = LaunchConfiguration('x', default='0.0')
    y = LaunchConfiguration('y', default='0.0')
    Y = LaunchConfiguration('Y', default='0.0')

    # Declare the launch arguments matching x/y/Y for compatibility with etrobo_world.launch.py
    declare_x_position_cmd = DeclareLaunchArgument(
        'x', default_value='0.0',
        description='Specify spawn x position of the robot')

    declare_y_position_cmd = DeclareLaunchArgument(
        'y', default_value='0.0',
        description='Specify spawn y position of the robot')

    declare_Y_position_cmd = DeclareLaunchArgument(
        'Y', default_value='0.0',
        description='Specify spawn yaw (rad) of the robot')

    start_gazebo_ros_spawner_cmd = Node(
        package='gazebo_ros',
        executable='spawn_entity.py',
        arguments=[
            '-entity', 'spike',
            '-file', urdf_path,
            '-x', x,
            '-y', y,
            '-z', '0.01',
            '-Y', Y
        ],
        output='screen',
    )

    ld = LaunchDescription()

    # Declare the launch options for x, y, Y so upstream launch files can pass these
    ld.add_action(declare_x_position_cmd)
    ld.add_action(declare_y_position_cmd)
    ld.add_action(declare_Y_position_cmd)

    # Add the actual spawn node
    ld.add_action(start_gazebo_ros_spawner_cmd)

    return ld
