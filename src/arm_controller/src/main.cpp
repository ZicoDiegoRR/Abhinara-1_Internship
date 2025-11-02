#include <ros/ros.h>
#include <sensor_msgs/JointState.h>
#include <cmath>
#include <vector>
#include <iostream>
#include <unistd.h>

// Length (yaw joint and links): 0.1 (yaw joint), 0.3 (first link), 0.3 (second link), 0.075 (third link)
const float max_x = 0.675;
const float min_x = -0.675;
float L1 = 0.1, L2 = 0.3, L3 = 0.3, L4 = 0.075;

std::vector<float> fKinematicsDeltaError(
    float theta1, float theta2, float theta3, std::vector<float> truth
) {
    float px = 0.3 + 0.3*cos(theta2) + 0.075*cos(theta2+theta3);
    float pz = 0.1 + 0.3*sin(theta2) + 0.075*sin(theta2+theta3);
    float py = px*sin(theta1);

    return {px - truth[0], py - truth[1], pz - truth[2]};
}

std::vector<float> iKinematics(float x, float y, float z) {
    float first_angle = atan2(y, x);

    float rem_x = sqrt(pow(x, 2) + pow(y, 2)) - L2;
    float rem_z = z - L1;

    float val = (
        pow(rem_x, 2) + pow(rem_z, 2) - pow(L3, 2) - pow(L4, 2)
    ) / (2 * L3 * L4);
    val = std::max(-1.0f, std::min(1.0f, val));
    float third_angle = acos(val);

    float second_angle = atan2(rem_z, rem_x) - atan2(
        (L4 * sin(third_angle)), (L3 + L4 * cos(third_angle))
    );
    
    return {first_angle, second_angle, third_angle};
}

float getZValuePseudoCircle(float x, float radius, float min_val) {
    if(x == 0) return radius;

    float r = pow(radius, 2) - pow(x, 2); // z = sqrt(r^2 - x^2)
    //std::cout << x << " " << r;
    if(r >= 0) return sqrt(r);
    else return 0;
}

bool in_range(float start_val, float stop_val, float verify) {
    float max_val = std::max(start_val, stop_val);
    float min_val = std::min(start_val, stop_val);

    if(verify >= min_val && verify <= max_val) return true;
    return false;
}

bool reachable(float x, float y, float z) {
    // Pseudo-circle
    float max_z = 0, min_z = 0, temp_x = x;
    if(x < 0) temp_x = x * (-1);
    if(temp_x <= max_x && temp_x >= min_x) {
        max_z = getZValuePseudoCircle(temp_x - L2, 0.375, 0.1) + L1;
        min_z = getZValuePseudoCircle(temp_x - L2, 0.225, 0.1) + L1;
        std::cout << max_z << " " << min_z << std::endl;
    } else {
        return false;
    }

    std::cout << in_range(min_z, max_z, z) << std::endl;

    if(!(in_range(min_z, max_z, z))) {
        return false;
    }

    float distance_from_center = sqrt(pow(x, 2) + pow(y, 2));
    std::cout << distance_from_center << std::endl; 
    if(!(in_range(max_x, min_x, x))) return false;

    return true;
}

int main(int argc, char** argv) {
        ros::init(argc, argv, "joint_state_publisher");
        ros::NodeHandle nh;
        ros::Publisher joint_state_pub = nh.advertise<sensor_msgs::JointState>("/joint_states", 10);
        ros::Rate loop_rate(10);

        float x, y, z;
        while(ros::ok()){
            sensor_msgs::JointState joint_state;
            joint_state.header.stamp = ros::Time::now();

            std::cout << "Enter x, y, and z coordinates: " << std::endl;
            std::cout << "(separated by a whitespace)" << std::endl;
            std::cin >> x >> y >> z;

            joint_state.name.resize(3);
            joint_state.position.resize(joint_state.name.size());
            if(reachable(x, y, z)) {
                std::vector<float> rad = iKinematics(x, y, z);
                std::cout << std::endl;
                for(int i = 0; i < rad.size(); i++) {
                    std::cout << "Angle " << i+1 << ": ";
                    std::cout << rad[i] << std::endl;
                }
                joint_state.name = {"bottom_joint", "lower_arm_joint", "upper_arm_joint"};

                std::vector<float> delta_error = fKinematicsDeltaError(
                    rad[0], rad[1], rad[2], {x, y, z}
                );
                std::cout << "Delta Error: ";
                for(float err : delta_error) std::cout << err << " ";

                joint_state.position = {
                    rad[0], rad[1], rad[2]
                }; //in radian

                joint_state_pub.publish(joint_state);
                std::cout << std::endl << "Joint state published!" << std::endl;
            } else {
                std::cout << "Robot can't reach that point :(" << std::endl;
            }

            ros::spinOnce();
            loop_rate.sleep();
        }
        
    return 0;
}