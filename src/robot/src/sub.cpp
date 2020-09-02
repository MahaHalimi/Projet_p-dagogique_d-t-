#include "ros/ros.h"
#include <sstream>
#include "geometry_msgs/Pose.h"

geometry_msgs::Pose pose;

void chatterCallback(const geometry_msgs::Pose::ConstPtr &msg) {
    pose = *msg;
}

void print() {
    //ROS_INFO("Crrent state : (%f, %f, %f)", pose.position.x, pose.position.y, pose.position.z);
    ROS_INFO("I heard: [%f]",pose.position.x);
}

int main(int argc, char **argv) {
    ros::init(argc, argv,"sub");
    ros::NodeHandle n;
    //ros::Subscriber sub = n.subscribe<geometry_msgs::Pose>("/vrep_ros_interface/pose", 1, &chatterCallback);
    ros::Subscriber sub = n.subscribe("pose", 100, chatterCallback);

ros::Rate loop_rate(100);
 while (ros::ok()){
    print();
    ros::spinOnce();
    loop_rate.sleep();   
    }

    return 0;
}