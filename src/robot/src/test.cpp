#include <ros/ros.h>
#include <geometry_msgs/Vector3.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Point.h>
#include <sensor_msgs/Joy.h>
#include <std_msgs/Float32.h>
#include <cmath>
#include <cstdlib>
#include <std_msgs/String.h>
#include "geometry_msgs/PoseWithCovarianceStamped.h"
#include <nav_msgs/Odometry.h>
#include <tf/transform_datatypes.h>
#include <tf/transform_broadcaster.h>

float current_x, current_y, current_theta;
int in = 0;
#define M_PI 3.14159265358979323846 /* pi */

void poseCallBack(const nav_msgs::Odometry::ConstPtr & msg) {
  current_x = msg - > pose.pose.position.x;
  current_y = msg - > pose.pose.position.y;
  current_theta = tf::getYaw(msg - > pose.pose.orientation);
  in = 1;
}

int main(int argc, char ** argv) {
  ros::init(argc, argv, "goal");
  ros::NodeHandle nh_;
  ros::Subscriber odom_sub = nh_.subscribe("/odom", 100, & poseCallBack);
  ros::Publisher cmd_vel_pub = nh_.advertise < geometry_msgs::Twist > ("/cmd_vel", 100);
  geometry_msgs::Twist cmd;
  double distance;
  ros::Rate loop_rate(100);
  geometry_msgs::Point goal;
  goal.x = -0.4266;
  goal.y = -0.7747;

  while (ros::ok()) {
    if ( in > 0) {
      double x = goal.x - current_x;
      double y = goal.y - current_y;
      distance = sqrt(pow(y, 2) + pow(x, 2)); // distance formula
      double angleToGoal = atan2(y, x); // angle
      ROS_INFO("Current angle: %f, Angle to goal: %f", current_theta * 180 / M_PI, angleToGoal * 180 / M_PI);

      if (distance < 0.05) // if goal is reach
      {
        cmd.linear.x = 0.0;

      } else {
        if (abs(angleToGoal - current_theta) > 0.1) // if angle isn't lined up
        {
          cmd.angular.z = 0.1;
        }
        // once it is lined up, it will go straight the next time it loop
        else {
          cmd.angular.z = 0.0;
          cmd.linear.x = 0.1; // linear speed              
        }
      }

      cmd_vel_pub.publish(cmd);
      loop_rate.sleep();
    }
    ros::spinOnce();
  }
  return 0;
}