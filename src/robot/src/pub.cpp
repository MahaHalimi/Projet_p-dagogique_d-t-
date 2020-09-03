#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "geometry_msgs/Pose.h"

geometry_msgs::Pose pose;
geometry_msgs::Twist msg;
double distance;

void chatterCallback(const geometry_msgs::Pose::ConstPtr &msg) {
    pose = *msg;
}


int main(int argc, char **argv){
ros::init(argc, argv, "move_robot");
ros::NodeHandle n;

ros::Subscriber sub = n.subscribe("pose", 100, chatterCallback);
ros::Publisher chatter_pub = n.advertise<geometry_msgs::Twist>("cmd_vel", 100);
ros::Rate loop_rate(10);


while (ros::ok()){


distance = sqrt(pow(pose.position.y, 2) + pow(pose.position.x, 2)); // distance formula

if (distance < 6)
{
  msg.linear.x = 0.7;
  msg.angular.z = 0.5;

}
else
{
  msg.linear.x = 0;
  msg.angular.z = 0;
}

chatter_pub.publish(msg);

ros::spinOnce();
loop_rate.sleep();

}
return 0;
}

