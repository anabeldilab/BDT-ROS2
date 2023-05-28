# Agent installation

## Installing micro-ROS agent

The micro-ROS agent is a bridge between the micro-ROS world and the ROS 2 world. It is in charge of translating the micro-ROS messages into ROS 2 messages and vice versa. It is also in charge of providing the micro-ROS client library with the required information to establish a connection with the micro-ROS app.

The micro-ROS agent is installed in the firmware workspace by default. However, if you want to install it in a different workspace, you can do it by executing the following command:

```bash
# Build micro-ROS tools and source them
colcon build
source install/local_setup.bash

# Install micro-ROS agent
ros2 run micro_ros_setup create_agent_ws.sh
```

## Building micro-ROS agent

Once the micro-ROS agent is installed, you can build it by executing the following command:

```bash
ros2 run micro_ros_setup build_agent.sh

# Run a micro-ROS agent with UDP transport
ros2 run micro_ros_agent micro_ros_agent udp4 --port 8888

# Run a micro-ROS agent with serial transport
ros2 run micro_ros_agent micro_ros_agent serial --dev /dev/ttyUSB0

```

