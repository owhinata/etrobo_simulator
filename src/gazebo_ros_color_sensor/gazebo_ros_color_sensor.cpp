// Copyright 2024 KatsumiOuwa
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// #include <cv_bridge/cv_bridge.h> // Remove cv_bridge include (not needed for just OpenCV)
#include <gazebo/common/Plugin.hh>
#include <gazebo/msgs/msgs.hh>
#include <gazebo/rendering/Camera.hh>
#include <gazebo/sensors/CameraSensor.hh>
#include <gazebo/sensors/Sensor.hh>
#include <gazebo/transport/Node.hh>
// #include <image_transport/image_transport.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <rclcpp/rclcpp.hpp>
#include <sdf/sdf.hh>
#include <sensor_msgs/msg/image.hpp>
#include <std_msgs/msg/color_rgba.hpp>

namespace gazebo_ros_color_sensor {

class GazeboRosColorSensor : public gazebo::SensorPlugin {
public:
  GazeboRosColorSensor() : gazebo::SensorPlugin() {}

  virtual ~GazeboRosColorSensor() {
    if (ros_spin_thread_.joinable()) {
      ros_spin_thread_.join();
    }
  }

  void Load(gazebo::sensors::SensorPtr _sensor, sdf::ElementPtr _sdf) {
    // Camera sensor pointer
    camera_sensor_ =
        std::dynamic_pointer_cast<gazebo::sensors::CameraSensor>(_sensor);
    if (!camera_sensor_) {
      gzerr << "GazeboRosColorSensor requires a CameraSensor.\n";
      return;
    }

    // ROS node
    node_ = rclcpp::Node::make_shared("gazebo_ros_color_sensor");

    color_pub_ = node_->create_publisher<std_msgs::msg::ColorRGBA>(
        "color", rclcpp::SensorDataQoS());

    last_stamp_sec_ = -1;
    connections_.push_back(camera_sensor_->Camera()->ConnectNewImageFrame(
        std::bind(&GazeboRosColorSensor::OnNewFrame, this,
                  std::placeholders::_1, std::placeholders::_2,
                  std::placeholders::_3, std::placeholders::_4,
                  std::placeholders::_5)));

    // Spin up thread for ROS2 node
    ros_spin_thread_ = std::thread([this] { rclcpp::spin(node_); });
  }

  void OnNewFrame(const unsigned char *_image, unsigned int _width,
                  unsigned int _height, unsigned int _depth,
                  const std::string & /*_format*/) {
    // Prevent flooding by only processing a new timestamp
    const gazebo::common::Time &stamp = camera_sensor_->LastMeasurementTime();
    if (stamp.Double() == last_stamp_sec_)
      return;
    last_stamp_sec_ = stamp.Double();

    // Convert raw data to cv::Mat
    unsigned int channels = _depth;
    cv::Mat image(_height, _width, (channels == 3 ? CV_8UC3 : CV_8UC1),
                  (void *)_image);
    cv::Vec3f avg_rgb(0, 0, 0);
    if (channels == 3) {
      cv::Scalar mean = cv::mean(image);
      avg_rgb = cv::Vec3f(mean[2], mean[1], mean[0]); // OpenCV uses BGR
    } else if (channels == 1) {
      double gray = cv::mean(image)[0];
      avg_rgb = cv::Vec3f(gray, gray, gray);
    } else {
      gzdbg << "Unsupported color channels in camera image: " << channels
            << "\n";
      return;
    }

    // Normalize values to 0-1
    std_msgs::msg::ColorRGBA color_msg;
    color_msg.r = avg_rgb[0] / 255.0f;
    color_msg.g = avg_rgb[1] / 255.0f;
    color_msg.b = avg_rgb[2] / 255.0f;
    color_msg.a = 1.0f;
    color_pub_->publish(color_msg);
  }

private:
  gazebo::sensors::CameraSensorPtr camera_sensor_;
  std::vector<gazebo::event::ConnectionPtr> connections_;
  std::shared_ptr<rclcpp::Node> node_;
  rclcpp::Publisher<std_msgs::msg::ColorRGBA>::SharedPtr color_pub_;
  std::thread ros_spin_thread_;
  double last_stamp_sec_;
};

// Register plugin with Gazebo
GZ_REGISTER_SENSOR_PLUGIN(gazebo_ros_color_sensor::GazeboRosColorSensor)

} // namespace gazebo_ros_color_sensor

// End of file
