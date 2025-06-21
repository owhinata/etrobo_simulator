cmake_minimum_required(VERSION 3.8)
project(etrobo_simulator)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)

install(
  DIRECTORY models worlds launch urdf
  DESTINATION share/${PROJECT_NAME}
)

ament_environment_hooks("${CMAKE_CURRENT_SOURCE_DIR}/gazebo_models_hook.sh.in")

# Added for gazebo_ros_color_sensor
add_library(gazebo_ros_color_sensor SHARED
  src/gazebo_ros_color_sensor/gazebo_ros_color_sensor.cpp
)
target_compile_features(gazebo_ros_color_sensor PUBLIC cxx_std_17)
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(gazebo_ros REQUIRED)
find_package(gazebo_dev REQUIRED)
find_package(gazebo REQUIRED)
find_package(std_msgs REQUIRED)

target_include_directories(gazebo_ros_color_sensor PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
target_link_libraries(gazebo_ros_color_sensor
  ${GAZEBO_LIBRARIES}
)
target_compile_definitions(gazebo_ros_color_sensor PUBLIC "PLUGINLIB__DISABLE_BOOST_FUNCTIONS")
ament_target_dependencies(gazebo_ros_color_sensor rclcpp sensor_msgs gazebo_ros std_msgs)

install(TARGETS gazebo_ros_color_sensor
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()

