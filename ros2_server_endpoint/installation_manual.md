# Server endpoint installation

 Download the [ROS2 branch of the ROS-TCP-Endpoint](https://github.com/Unity-Technologies/ROS-TCP-Endpoint/tree/main-ros2) repository and copy it into the src folder in your Colcon workspace. Then navigate to your Colcon workspace and run the following commands:

```bash
source install/setup.bash
colcon build
source install/setup.bash
```
In your Colcon workspace, run the following command, replacing <your IP address> with your ROS machine's IP or hostname.

```bash
ros2 run ros_tcp_endpoint default_server_endpoint --ros-args -p ROS_IP:=<your IP address>
```

On Linux you can find out your IP address with the command hostname -I.
