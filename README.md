# Control of Electromechanical Devices using BCI: ROS projects

## ROS nodes Setup
- ROS publisher node (Unity)
- ROS Agent node
- ROS subscriber node (Microros)

## Configure ROS on the microcontroller
```bash
# Create a workspace and download the micro-ROS tools
source /opt/ros/$ROS_DISTRO/setup.bash
git clone -b $ROS_DISTRO https://github.com/micro-ROS/micro_ros_setup.git src/micro_ros_setup

# Update dependencies using rosdep
sudo apt update && rosdep update
rosdep install --from-paths src --ignore-src -y

# Build micro-ROS tools and source them
sudo apt install python3-colcon-common-extensions
colcon build
source install/local_setup.bash
```
### Client 

```bash
# Create firmware step
ros2 run micro_ros_setup create_firmware_ws.sh host

``` 

Once the command is executed, a folder named 'firmware' should be present in your workspace. This step takes care of, among other things, downloading a set of micro-ROS applications for Linux, which are located at src/uros/micro-ROS-demos/rclc. Each application is represented by a folder that contains the following files:
main.c: This file contains the logic of the application.
CMakeLists.txt: This is the CMake file that contains the script to compile the application.

To create a custom application, the user should register a folder <my_app> in this location, containing the two files just described. Additionally, any new application folder must be registered in *src/uros/micro-ROS-demos/rclc/CMakeLists.txt* by adding the following line:
*export_executable(<my_app>)*


```bash
# Build step
ros2 run micro_ros_setup build_firmware.sh
source install/local_setup.bash 
```
### Agent

```bash
# Download micro-ROS-Agent packages
ros2 run micro_ros_setup create_agent_ws.sh

# Build step
ros2 run micro_ros_setup build_agent.sh
source install/local_setup.bash
```

## Run nodes
```bash
# Run a micro-ROS agent
ros2 run micro_ros_agent micro_ros_agent udp4 --port 8888

# Serial connection
ros2 run micro_ros_agent micro_ros_agent serial --dev /dev/ttyUSB0
```
