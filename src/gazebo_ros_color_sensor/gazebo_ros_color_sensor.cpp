// Minimal Gazebo ROS Color Sensor Plugin (example)
#include <gazebo/common/Plugin.hh>
#include <gazebo/rendering/Camera.hh>
#include <gazebo/sensors/CameraSensor.hh>
#include <gazebo/sensors/Sensor.hh>
#include <gazebo/transport/transport.hh>
#include <memory>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <string>

namespace gazebo_ros_color_sensor {

class GazeboRosColorSensor : public gazebo::SensorPlugin {
public:
  GazeboRosColorSensor() : gazebo::SensorPlugin() {}

  virtual ~GazeboRosColorSensor() {
    rclcpp::shutdown();
    if (ros_thread_.joinable()) {
      ros_thread_.join();
    }
  }

  void Load(gazebo::sensors::SensorPtr _sensor, sdf::ElementPtr _sdf) override {
    // Connect to CameraSensor
    auto cam_sensor =
        std::dynamic_pointer_cast<gazebo::sensors::CameraSensor>(_sensor);
    if (!cam_sensor) {
      gzerr << "GazeboRosColorSensor: Not a CameraSensor!\n";
      return;
    }

    // Connect to camera update event
    cam_new_frame_conn_ = cam_sensor->Camera()->ConnectNewImageFrame(std::bind(
        &GazeboRosColorSensor::OnNewFrame, this, std::placeholders::_1,
        std::placeholders::_2, std::placeholders::_3, std::placeholders::_4,
        std::placeholders::_5));

    // Create ROS node
    rclcpp::init(0, nullptr);
    node_ = rclcpp::Node::make_shared("gazebo_color_sensor");
    pub_ = node_->create_publisher<sensor_msgs::msg::Image>(
        "/color_sensor/image_raw", 10);

    // Start thread for ROS spinning
    ros_thread_ = std::thread([&]() { rclcpp::spin(node_); });
  }

  void OnNewFrame(const unsigned char *image, unsigned int width,
                  unsigned int height, unsigned int depth,
                  const std::string &format) {
    auto msg = sensor_msgs::msg::Image();
    msg.height = height;
    msg.width = width;
    msg.encoding = "rgb8";
    msg.step = depth * width;
    msg.data.assign(image, image + (depth * width * height));
    pub_->publish(msg);
  }

private:
  rclcpp::Node::SharedPtr node_;
  rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr pub_;
  gazebo::event::ConnectionPtr cam_new_frame_conn_;
  std::thread ros_thread_;
};

GZ_REGISTER_SENSOR_PLUGIN(gazebo_ros_color_sensor::GazeboRosColorSensor)

} // namespace gazebo_ros_color_sensor
