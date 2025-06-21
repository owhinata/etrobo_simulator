# Repository Guidelines

This repository provides a ROS2 + Gazebo simulator environment for ET Robocon.

## Environment
- Ubuntu 22.04
- ROS2 Humble
- Gazebo 11.10.2
- Python 3.10

## Development
- Prefer Python when possible. Use C++ only for performance-critical parts.
- Models are provided as SDF (Gazebo) and URDF (ROS2).
- When developing new features, make the minimum necessary changes only. Do not perform refactoring such as variable renaming or other code improvements unless specifically instructed.

## Documentation
- Write documentation, code comments, and branch names in English.

## Testing
- Run `colcon test --packages-select etrobo_simulator` after making changes.

