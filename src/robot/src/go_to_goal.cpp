#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "geometry_msgs/Pose.h"
#include <tf/transform_datatypes.h>
#include <tf/transform_broadcaster.h>

geometry_msgs::Pose pose;
geometry_msgs::Twist msg;
double distance,angleToGoal,x,y;
float current_x, current_y, current_theta;

void chatterCallback(const geometry_msgs::Pose::ConstPtr &msg) {
    pose = *msg;
  current_x = pose.position.x;
  current_y = pose.position.y;
  current_theta = tf::getYaw(pose.orientation);
}


int main(int argc, char **argv){
ros::init(argc, argv, "move_robot");
ros::NodeHandle n;

ros::Subscriber sub = n.subscribe("pose", 100, chatterCallback);
ros::Publisher chatter_pub = n.advertise<geometry_msgs::Twist>("cmd_vel", 100);
ros::Rate loop_rate(10);

geometry_msgs::Point goal;

  goal.x =  6.0000;
  goal.y = -2.0500;

while (ros::ok()){

 x = goal.x - current_x;
 y = goal.y - current_y;
distance = sqrt(pow(y, 2) + pow(x, 2)); // distance formula
angleToGoal = atan2(y, x); // angle

//ROS_INFO("Current angle: %f, Angle to goal: %f", current_theta* 180 / M_PI, angleToGoal* 180 / M_PI);
ROS_INFO("distance: %f", distance);
//ROS_INFO("abs(angleToGoal - current_theta) : %f", abs(angleToGoal - current_theta) );

msg.angular.z = -2.5;
//ROS_INFO("current_theta: %f", current_theta);

if (current_theta < 2.250239) 
      {
        //ROS_INFO("done");
        msg.angular.z = 0.0;
        msg.linear.x = 1.0;
      } 
while (distance < 0.394215)
       { 
        msg.linear.x = 0.0;
       }       
          

/*
if (distance < 0.05) // if goal is reach
      {
        msg.linear.x = 0.0;

      } else {
        if (distance <0.394215) // if angle isn't lined up
        {
          msg.linear.x = 0.0;
        }
        // once it is lined up, it will go straight the next time it loop
        else {
          msg.angular.z = 0.0;
          msg.linear.x = 2.1; // linear speed              
        }
      }*/
chatter_pub.publish(msg);

ros::spinOnce();
loop_rate.sleep();

}
return 0;
}

