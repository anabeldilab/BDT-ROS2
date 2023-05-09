# Client installation

## Installing ROS 2 and the micro-ROS build system

Once you have a ROS 2 installation in the computer, follow these steps to install the micro-ROS build system:

```bash
# Source the ROS 2 installation
source /opt/ros/$ROS_DISTRO/setup.bash

# Create a workspace and download the micro-ROS tools
mkdir microros_ws
cd microros_ws
git clone -b $ROS_DISTRO https://github.com/micro-ROS/micro_ros_setup.git src/micro_ros_setup

# Update dependencies using rosdep
sudo apt update && rosdep update
rosdep install --from-paths src --ignore-src -y

# Install pip
sudo apt-get install python3-pip

# Build micro-ROS tools and source them
colcon build
source install/local_setup.bash

```

These instructions will setup a workspace with a ready-to-use micro-ROS build system. This build system is in charge of downloading the required cross-compilation tools and building the apps for the required platforms.

The build system’s workflow is a four-step procedure:

- _Create step:_ This step is in charge of downloading all the required code repositories and cross-compilation toolchains for the specific hardware platform. Among these repositories, it will also download a collection of ready to use micro-ROS apps.

- _Configure step:_ In this step, the user can select which app is going to be cross-compiled by the toolchain. Some other options, such as transport, agent’s IP address/port (for UDP transport) or device ID (for serial connections) will be also selected in this step.
  Build step: Here is where the cross-compilation takes place and the platform-specific binaries are generated.

- _Flash step:_ The binaries generated in the previous step are flashed onto the hardware platform memory, in order to allow the execution of the micro-ROS app. Further information about micro-ROS build system can be found here.

Once the build system is installed, let’s create a firmware workspace that targets all the required code and tools:

```bash
# Create firmware step
ros2 run micro_ros_setup create_firmware_ws.sh freertos esp32
```

Once the command is executed, a folder named firmware must be present in your workspace.

This step is in charge, among other things, of downloading a set of micro-ROS apps for the specific platform you are addressing. In the case of FreeRTOS, these are located at firmware/freertos_apps/apps. Each app is represented by a folder containing the following files:

app.c: This file contains the logic of the application.
app-colcon.meta: This file contains the micro-ROS app specific colcon configuration. Detailed info on how to configure the RMW via this file can be found here.
For the user to create its custom application, a folder <my_app> will need to be registered in this location, containing the two files just described.

Additionally, any new application folder must be registered in firmware/freertos_apps/apps/

```bash
# Configure step
ros2 run micro_ros_setup configure_firmware.sh string_subscriber -t serial --dev /dev/ttyUSB0

# Build step
ros2 run micro_ros_setup build_firmware.sh
```

The configure step is in charge of selecting the app to be cross-compiled, as well as the transport to be used. In this case, the ping_pong app will be cross-compiled for the serial transport, using the /dev/ttyUSB0 port.

The build step will cross-compile the app and generate the binary file that will be flashed onto the hardware platform.

```bash
# Flash step
ros2 run micro_ros_setup flash_firmware.sh
```

The flash step will flash the binary file generated in the previous step onto the hardware platform. Once the flashing process is finished, the micro-ROS app will be running on the hardware platform.
