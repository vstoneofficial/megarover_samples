#include <ros/ros.h>
#include <sensor_msgs/Joy.h>
#include <geometry_msgs/Twist.h>


class JoyCtrlMegarover
{
public:
  JoyCtrlMegarover();

private:
  void joyCallback(const sensor_msgs::Joy::ConstPtr& joy);

  ros::NodeHandle nh_;

  int linear_, angular_, safety_;
  double l_scale_, a_scale_;
  ros::Publisher vel_pub_;
  ros::Subscriber joy_sub_;

};


JoyCtrlMegarover::JoyCtrlMegarover():
  linear_(1),
  angular_(0),
  a_scale_(0.8),
  l_scale_(0.6),
  safety_(2)
{
  nh_.param("/joycon/axis_linear", linear_, linear_);
  nh_.param("/joycon/axis_angular", angular_, angular_);
  nh_.param("/joycon/scale_angular", a_scale_, a_scale_);
  nh_.param("/joycon/scale_linear", l_scale_, l_scale_);
  nh_.param("/joycon/safety_button", safety_, safety_);

  vel_pub_ = nh_.advertise<geometry_msgs::Twist>("rover_twist", 1);
  joy_sub_ = nh_.subscribe<sensor_msgs::Joy>("joy", 1, &JoyCtrlMegarover::joyCallback, this);
}

void JoyCtrlMegarover::joyCallback(const sensor_msgs::Joy::ConstPtr& joy)
{
  geometry_msgs::Twist twist;
  if(joy->buttons[safety_]){
    twist.angular.z = a_scale_*joy->axes[angular_];
    twist.linear.x = l_scale_*joy->axes[linear_];
  }else{
    twist.angular.z = 0;
    twist.linear.x = 0;
  }
  vel_pub_.publish(twist);
}


int main(int argc, char** argv)
{
  ros::init(argc, argv, "joycon");
  JoyCtrlMegarover joy_ctrl_megarover;

  ros::NodeHandle n;

  ros::Rate r(10);
  while(n.ok()){
    ros::spinOnce();
    r.sleep();
  }

  return 0;
}
