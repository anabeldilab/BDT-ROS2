# Publisher node installation

## Prepare the workspace

Best practice is to create a new directory for every new workspace. The name doesn’t matter, but it is helpful to have it indicate the purpose of the workspace. Let’s choose the directory name ros2_ws, for “development workspace”:

```bash
# Create a workspace 
mkdir microros_ws
cd microros_ws
```

Navigate into ros2_ws/src, and run the package creation command:

```bash
ros2 pkg create --build-type ament_cmake cpp_pubsub
```

Your terminal will return a message verifying the creation of your package cpp_pubsub and all its necessary files and folders.

Navigate into ros2_ws/src/cpp_pubsub/src. Recall that this is the directory in any CMake package where the source files containing executables belong.

```bash
# Download the example talker code
wget -O publisher_member_function.cpp https://raw.githubusercontent.com/ros2/examples/foxy/rclcpp/topics/minimal_publisher/member_function.cpp
```

## Add dependencies

Navigate one level back to the ros2_ws/src/cpp_pubsub directory, where the CMakeLists.txt and package.xml files have been created for you.

Open package.xml with your text editor.

As mentioned in the previous tutorial, make sure to fill in the `<description>`, `<maintainer>` and `<license>` tags:
```xml
<description>Examples of minimal publisher/subscriber using rclcpp</description>
<maintainer email="you@email.com">Your Name</maintainer>
<license>Apache License 2.0</license>
```
Add a new line after the ament_cmake buildtool dependency and paste the following dependencies corresponding to your node’s include statements:
```xml
<depend>rclcpp</depend>
<depend>std_msgs</depend>
```
This declares the package needs rclcpp and std_msgs when its code is built and executed.

Make sure to save the file.

## CMakeLists.txt
Now open the CMakeLists.txt file. Below the existing dependency find_package(ament_cmake REQUIRED), add the lines:

```cmake
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
```
After that, add the executable and name it talker so you can run your node using ros2 run:

```cmake
add_executable(talker src/publisher_member_function.cpp)
ament_target_dependencies(talker rclcpp std_msgs)
```
Finally, add the install(TARGETS...) section so ros2 run can find your executable:
```cmake
install(TARGETS
  talker
  DESTINATION lib/${PROJECT_NAME})
```

# Build and run the publisher node

```bash
rosdep install -i --from-path src --rosdistro foxy -y

colcon build --packages-select cpp_pubsub

. install/setup.bash

ros2 run cpp_pubsub talker_string
```


# String type publisher no works

String type publisher no works, because the message type seems like it is not supported by micro-ROS. There are no errors in the compilation, but when the node is executed, the subscriber does not receive any message.

https://micro.ros.org/blog/2021/07/21/handlingmemorymessages/
Handling memory messages Tutorial for micro-ROS

Having total awareness of what ROS 2 messages and types are being used for, is essential in order to handle memory correctly in an embedded C99 environment.

The following Tutorial “Handling messages memory in micro-ROS ‘’ at icro-ROS web page explains in detail how to handle messages and types memory in micro-ROS.

The two approaches of micro-ROS memory handling are present in this tutorial.

micro-ROS Foxy: Memory handling using traditional allocation approach.

micro-ROS Galactic: Memory handling using type support introspection.

Type Support introspection in C in micro-ROS Galactic distribution enables the new type handling API. This package is able to auto-assign memory to a certain message structure using default dynamic memory allocators.

Thanks to the inclusion of this feature, an automated memory handling for micro-ROS types is available, enhancing the usability of the micro-ROS API.

The tools related to this feature are available in the package micro_ros_utilities and the documentation is available here. Check this example for more information about how to avoid the message memory initialization problems in micro-ROS.

https://docs.vulcanexus.org/en/latest/rst/microros_documentation/getting_started/getting_started.html






