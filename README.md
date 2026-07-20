# Autonomous Emergency Braking (AEB) System

This project implements a basic Autonomous Emergency Braking (AEB) system simulation using ROS 2 (Robot Operating System) and C++. It consists of two main nodes that communicate via the ROS 2 publish/subscribe architecture.

## System Components

* RadarNode (radar_node): Simulates a radar sensor by calculating the positions of both the vehicle and an obstacle over time. It publishes the relative distance between them to the radar_distance topic.
* AebNode (aeb_node): Acts as the "brain" of the system. It subscribes to the radar_distance topic and processes the incoming data to monitor for potential collision risks.

## Prerequisites

* ROS 2 (Humble or newer recommended)
* Ubuntu (or WSL2 on Windows)
* colcon build tool

## Building the Project

1. Navigate to your ROS 2 workspace:
```bash
   cd ~/ros2_ws
```

2. Build the package:
```bash
   colcon build --packages-select aeb_system
```

3. Source the environment:
```bash
   source install/setup.bash
```

## Running the Nodes

Open two separate terminals and run the nodes independently:

1. Start the Radar:
```bash
   ros2 run aeb_system radar_node
```

2. Start the AEB Brain:
```bash
   ros2 run aeb_system aeb_node
```

The AEB node will start logging the received distance values directly from the radar in real-time.