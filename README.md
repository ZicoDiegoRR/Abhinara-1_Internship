# Implementing Inverse Kinematics to A Robot Arm Model Leveraging ROS Noetic

In this repository, I made a simple ROS source code to 
display a robot arm and a C++ code to move it with given end effector.

## Author's Profile

Name: Zico Diego Rio Ramadhonny

NRP : 5054251023

## Setup (How to Use)

First, make sure you have ROS Noetic installed.

Second, clone this repository.
```bash
git clone --single-branch --branch Day-5 https://github.com/ZicoDiegoRR/Abhinara-1_Internship.git
```

Third, change your working directory to the cloned repository.
```bash
cd <path/to/repo>
```

Fourth, compile the source code using `catkin_make`.
```bash
catkin_make
```

Fifth, initiate `roscore` in different terminal.
```bash
roscore
```

Sixth, go back to the previous terminal and source the setup.
```bash
source devel/setup.sh
```

Seventh, initiate RViz by using this command.
```bash
roslaunch robot_description display.launch
```

Eighth, create another terminal, repeat the sixth step on the new terminal, and initiate the `arm_controller` node.
```bash
source devel/setup.sh
rosrun arm_controller arm_controller
```

Enjoy!
