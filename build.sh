#!/bin/bash

set -e

cd ~/ros2_ws

colcon build --packages-select aeb_system

source install/setup.bash

tmux kill-session -t aeb_session 2>/dev/null || true

tmux new-session -d -s aeb_session 'source $HOME/ros2_ws/install/setup.bash && ros2 run aeb_system radar_node --ros-args --params-file $HOME/ros2_ws/src/cpp-autonomous-braking-ros2/aeb_system/config/aeb_params.yaml; exec bash'

tmux split-window -h 'source $HOME/ros2_ws/install/setup.bash && ros2 run aeb_system aeb_node --ros-args --params-file $HOME/ros2_ws/src/cpp-autonomous-braking-ros2/aeb_system/config/aeb_params.yaml; exec bash'

tmux attach-session -t aeb_session