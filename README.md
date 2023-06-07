# Bachelor's Degree Thesis: Control of Electromechanical Devices using BCI: ROS projects

## About
This project is about controlling of electromechanincal devices using BCI. The project is divided in two parts:
- ROS nodes
- Unity project

The ROS nodes are responsible for the communication between the Unity project and the microcontroller. The Unity project is responsible for the communication between the ROS nodes and the BCI. The microcontroller is responsible for the communication between the ROS nodes and the electromechanical devices.

## Requirements
- [ROS 2](https://docs.ros.org/en/rolling/Releases/Release-Humble-Hawksbill.html)
- [Unity](https://unity.com/)
- [ESP32-WROOM-32D](https://www.espressif.com/en/products/socs/esp32)
- [micro-ROS](https://micro.ros.org/)
- [NextMind](https://www.next-mind.com/)
- [Servomotor](https://en.wikipedia.org/wiki/Servomotor)


## Versions
- ROS 2 Humble Hawksbill
- Unity 2020.3.14f1 
- Ubuntu 22.04.2 LTS 
- ESP32-WROOM-32D 

## ROS nodes Setup
- ROS publisher node (Unity)
- ROS Agent node
- ROS subscriber node (Microros)

### ROS publisher node (Unity)
The ROS publisher node is responsible for the communication between the Unity project and the microcontroller. The node is responsible for publishing the data from the BCI to the microcontroller. The node is implemented in C# using the [Unity-Robotics-Hub](https://github.com/Unity-Technologies/Unity-Robotics-Hub)

### ROS Agent node
The ROS Agent node is responsible for the communication between the ROS publisher node and the ROS subscriber node. The node is implemented in C using the [micro-ROS](https://micro.ros.org/)

### ROS subscriber node (Microros)
The ROS subscriber node is responsible for the communication between the ROS Agent node and the microcontroller. The node is responsible for subscribing to the data from the ROS Agent node and sending the data to the microcontroller. The node is implemented in C using the [micro-ROS](https://micro.ros.org/)


