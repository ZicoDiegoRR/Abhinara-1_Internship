#include "ros/ros.h"
#include "sensor_msgs/JointState.h"
#include "control_arm/pub_sub.h"
#include "std_msgs/String.h"
#include <unistd.h>
#include <iostream>
#include <vector>
#include <cmath>

// 10.876 + 0
const float L1 = 15.711, L2 = 11.976, L3 = 14.3, L4 = 11.902, pris = 30;
const float block_height = 5.5, block_width = 3.99, arm_length = L2 + L3 + L4;
const int x_stack = 0, y_stack = 30;
const float sensor_to_grip = 3.75;

int step = 1;
bool is_scanning = true, stop = false;
std::vector<std::vector<float>> blocks;
std::vector<float> rads = {0, 0, 0, 0, 0, 0};
std::vector<float> rads_trajectory = {0, 0, 0, 0, 0, 0};

float euclidan(float x_dest, float y_dest, float z_dest, 
    float x_from, float y_from, float z_from) {
    
    float x_dist = std::pow(x_dest - x_from, 2);
    float y_dist = std::pow(y_dest - y_from, 2);
    float z_dist = std::pow(z_dest - z_from, 2);
    return std::sqrt(x_dist + y_dist + z_dist);
}

std::vector<float> iKi(float end_x, float end_y, float end_z, bool close_grip) {
    // Check if catching block
    float rad_grip;
    if(close_grip) rad_grip = 1.5708; else rad_grip = 0;

    // Measure the distance from the arm (excluding z axis)
    float euc_x_y = euclidan(end_x, end_y, 0, rads[0]*300, 0, 0);
    if (euc_x_y > arm_length) {
        // Measuring the x distance
        float arm_reduction;
        if(is_scanning) arm_reduction = 7.5; else arm_reduction = L3;
        float x_new, x_change = std::sqrt(
            std::pow(arm_length, 2) - std::pow(end_y, 2)
        ) - arm_reduction;

        // If too far from the robot's reach, doesn't change anything
        if(std::isnan(x_change)) {
            x_new = rads[0];
        // Or else, calculate (1:300 scale (URDF:Real))
        } else {
            //std::cout << end_x - x_change << std::endl;

            //std::cout << x_new << std::endl;
            if(end_x < rads[0]*300) x_new = (end_x + x_change)/300;
            else if(end_x > rads[0]*300) x_new = (end_x - x_change)/300;
            //std::cout << x_new << " " << x_change << std::endl;
        }

        return {x_new, rads[1], rads[2], rads[3], rads[4], rad_grip};
    } else {
        // When it's reachable, we break the 3-DOF problem into two
        float first_part_length = euc_x_y - L4;
        float end_z_first = end_z - L1;
        //std::cout << first_part_length << " " << end_z_first << std::endl;

        float third_degree, fourth_degree, fifth_degree;

        // Checking if the third and the fourth joints can reach it
        // If not, doesn't change anything
        float dist_check = std::pow(first_part_length, 2) + std::pow(end_z_first, 2);
        float mindist_for_1st_part = std::pow(L2, 2) + std::pow(L3, 2);
        if(dist_check < mindist_for_1st_part) {
            third_degree = rads[2];
            fourth_degree = rads[3];
        // If possible, calculate
        } else {
            float cos_val = (
                std::pow(first_part_length, 2) + std::pow(end_z_first, 2)
                - (std::pow(L2, 2) + std::pow(L3, 2))
            )/(2*L2*L3);
            float clamp_cos_val = std::min(1.0f, std::max(-1.0f, cos_val));
            fourth_degree = std::acos(clamp_cos_val);
            //std::cout << fourth_degree << " " << cos_val << std::endl;

            float first_third_deg = std::atan2(end_z_first, first_part_length);
            float second_third_deg = std::atan2(
                (L3 * std::sin(fourth_degree)), 
                (L2 + L3 * std::cos(fourth_degree))
            );
            //std::cout << first_third_deg << " " << second_third_deg << std::endl;

            third_degree = first_third_deg - second_third_deg;
                
            // Checking if the previous calculation resulted in negative radian for
            // the third joint
            // If yes, change the solution to be elbow-up
            if(third_degree < 0) {
                fourth_degree *= -1;
                first_third_deg = std::atan2(end_z_first, first_part_length);
                second_third_deg = std::atan2(
                    (L3 * std::sin(fourth_degree)), 
                    (L2 + L3 * std::cos(fourth_degree))
                );

                third_degree = first_third_deg - second_third_deg;
                if(third_degree < 0) {
                    third_degree = 0;
                    fourth_degree = std::atan2(end_z_first, first_part_length - L2);
                }
            }
        }

        // We use the top view of the arm now (using x and y axis)
        float second_degree = std::atan2(end_y, end_x - rads[0]*300);

        // Check if it's on the other side
        //std::cout << second_degree << " " << third_degree << std::endl;
        if(end_y < 0) {
            fourth_degree *= -1;
            third_degree = 3.14159 - third_degree;
            second_degree += 3.14159;
            fifth_degree = 3.14159 - (third_degree + fourth_degree);
        } else {
            fifth_degree = -(third_degree + fourth_degree);
        }

        return {
            rads[0], 
            second_degree, 
            third_degree, 
            fourth_degree, 
            fifth_degree,
            rad_grip,
        };
    }
}

std::vector<std::vector<float>> run_arm() {
    std::vector<std::vector<float>> rad_result;
    if(is_scanning) {
        rad_result = {
            {0, 1.5708, 3.14159, 1.5708, -1.5708, 1.5708, 0},
            {0.3, 1.5708, 3.14159, 1.5708, -1.5708, 1.5708, 0},
            {0.3, 3.14159, 3.14159, 1.5708, -1.5708, 1.5708, 0}, // Geometry
            {0.3, 1.5708, 3.14159, 1.5708, -1.5708, 1.5708, 0},
            {0.3, 1.5708, 0, -1.5708, 1.5708, 1.5708, 0},
            {0.3, 0, 0, -1.5708, 1.5708, 1.5708, 0}, // Geometry
            {0.3, 1.5708, 0, -1.5708, 1.5708, 1.5708, 0},
            {0, 1.5708, 0, -1.5708, 1.5708, 1.5708, 0}
        };
    } else {
        stop = false;
        std::vector<std::string> pick_step = {
            "go", "pick", "go_up", "stack", "drop"
        };

        std::vector<float> rads_copy = rads;
        for(int c = 0; c < 3; c++) {
            std::vector<float> b_select = blocks[blocks.size() - c - 1], calc_rad;
            for(int iter = 0; iter < pick_step.size(); iter++) {
                float x_calc = b_select[0], y_calc = b_select[1], z_calc = block_height/2;
                bool open_grip = false;

                if(pick_step[iter] == "go_up") {
                    z_calc *= 2 * (c + 1);
                } else if(pick_step[iter] == "stack" || pick_step[iter] == "drop") {
                    x_calc = x_stack; y_calc = y_stack; z_calc *= 2 * (c + 1);
                    if(pick_step[iter] == "drop") open_grip = true;
                } else if(pick_step[iter] == "go") {
                    open_grip = true;
                }
                calc_rad = iKi(x_calc, y_calc, z_calc, open_grip);
                rad_result.push_back(calc_rad);
                if(calc_rad[0] != rads[0]) {
                    rads = calc_rad;
                    calc_rad = iKi(x_calc, y_calc, z_calc, open_grip);

                    rad_result.push_back(calc_rad);
                }
            }
        }
        rads = rads_copy;
    }
    return rad_result;
}

void move_arm(std::vector<float> state, int frame, 
    const ros::Publisher& pub, ros::Rate& loop_rate,
    const ros::Publisher& pub_2) {

    if(stop) ROS_INFO("Stopped."); else ROS_INFO("Publishing the joint states...");

    for(int i = 0; i < state.size(); i++) printf(
        "Joint state %i: %.2f\n", i+1, state[i]
    );
    std::cout << std::endl;

    std::vector<float> increment;
    for(int i = 0; i < state.size(); i++) {
        increment.push_back((state[i] - rads[i])/frame);
    }

    std::vector<float> trajectory;
    for(int j = 0; j < frame; j++) {
        std::vector<double> trajectory_double;
        std::string serial_data;
        if(blocks.size() != 3) serial_data = "SEND:";
        else serial_data = "CMMD:";
        if(!stop) {
            sensor_msgs::JointState joints;
            joints.header.stamp = ros::Time::now();
            joints.name.resize(7);
            joints.position.resize(joints.name.size());
            joints.name = {
                "1.1_prismatic", "1.1", "1.2",
                "1.3", "1.4", "L1", "L2"
            };
            std_msgs::String msg;

            trajectory = {
                rads[0] + increment[0] * (j + 1), 
                rads[1] + increment[1] * (j + 1), 
                rads[2] + increment[2] * (j + 1), 
                rads[3] + increment[3] * (j + 1), 
                rads[4] + increment[4] * (j + 1), 
                state[5], 
                0
            };
            for(int i = 0; i < trajectory.size(); i++) {
                trajectory_double.push_back(static_cast<double>(trajectory[i]));

                if(i != trajectory.size() - 1) serial_data += std::to_string(trajectory[i]) + ",";
            }
            msg.data = serial_data + std::to_string(step);

            joints.position = trajectory_double;
            rads_trajectory = trajectory;

            pub.publish(joints);
            pub_2.publish(msg);
        }
        ros::spinOnce();
        loop_rate.sleep();
    }
    rads = trajectory;
}

void callback_for_block(const control_arm::pub_sub::ConstPtr& msg) {
    if(is_scanning && (msg->step != 7 && msg->step != 4) && msg->is_block) {
        bool to_add = false; float x_add, y_add, x_check, y_check, current_rad; // 3 and 6
        y_add = L2 + L4 + msg->y - sensor_to_grip;
        x_add = msg->x;
        if(msg->step == 3 || msg->step == 6) {
            if(msg->step == 3) current_rad = 3.14159 - rads_trajectory[1];
            else current_rad = rads_trajectory[1];
            y_add *= std::sin(current_rad);
            x_add = y_add * std::cos(current_rad) + pris;
        }
        if(msg->step < 4) y_add *= -1;

        if(blocks.empty()) to_add = true;
        else {
            std::vector<float> last_block = blocks[blocks.size() - 1];

            x_check = std::abs(x_add - last_block[0]);
            y_check = std::abs(y_add - last_block[1]);
            if(y_check > block_width || x_check > block_width) to_add = true;
        }

        if(to_add) {
            blocks.push_back({x_add, y_add});
            printf("Found a block at (%f, %f) coordinate (xy axis)", x_add, y_add);
        }
    }
    if(blocks.size() == 3) {is_scanning = false; step = 1; stop = true;}
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "joint_state_publisher");
    ros::NodeHandle n;

    ros::Subscriber sub = n.subscribe("joint_calc", 10, callback_for_block);
    ros::Publisher pub = n.advertise<sensor_msgs::JointState>("/joint_states", 10);
    ros::Publisher pub_2 = n.advertise<std_msgs::String>("joint_move", 10);
    ros::Rate loop_rate(25);

    int frame;
    std::string mode;
    std::cout << "Mode? (type 'manual', 'scan', 'stack', or 'run')" << std::endl;
    std::getline(std::cin, mode);

    while(ros::ok()) {
        //std::cout << rads[0] << std::endl;
        if(mode == "manual") {
            float x, y, z;
            std::vector<float> state;
            frame = 20;
            is_scanning = false;

            std::cout << "Input the end effector: ";
            std::cin >> x >> y >> z;
            state = iKi(x, y, z, false);
            move_arm(state, frame, pub, loop_rate, pub_2);
            rads = state;
        } else if(mode == "scan") {
            is_scanning = true;
            frame = 100;
            std::vector<std::vector<float>> states;
            states = run_arm();

            for(int i = 0; i < states.size(); i++) {
                //if(!stop) {
                    move_arm(states[i], frame, pub, loop_rate, pub_2);
                    //if(step != 8) step += 1; else stop = true;
                //} else {
                //    break;
                //}
            }
        } else if(mode == "stack") {
            float x, y;
            frame = 40;
            is_scanning = false;
            std::vector<std::vector<float>> states;

            std::cout << "Input the x and y coordinates for all blocks (three times):";
            std::cout << std::endl;

            for(int i = 0; i < 3; i++) {
                std::cin >> x >> y;
                blocks.push_back({x, y});
            }
            states = run_arm();
            for(int i = 0; i < states.size(); i++) {
                if(!stop) {
                    move_arm(states[i], frame, pub, loop_rate, pub_2);
                    sleep(0.5);
                } else {
                    break;
                }
            }
        } else if(mode == "run") {
            frame = 40;
            is_scanning = true;
            std::vector<std::vector<float>> states;

            // Scan
            states = run_arm();
            for(int i = 0; i < states.size(); i++) {
                if(!stop) {
                    move_arm(states[i], frame, pub, loop_rate, pub_2);
                    if(step != 8) step += 1; else stop = true;
                } else {
                    break;
                }
            }

            // Stack
            frame = 40;
            states = run_arm();
            for(int i = 0; i < states.size(); i++) {
                move_arm(states[i], frame, pub, loop_rate, pub_2);
                sleep(0.5);
            }
        }
        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}