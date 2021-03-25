#include <ros/ros.h>
#include <actionlib/server/simple_action_server.h>
#include <wei_goto/wei_gotoAction.h>
#include <tf/transform_listener.h>
class WEI_GOTO
{
protected:

  ros::NodeHandle nh_;
  ros::Publisher pub_;
  actionlib::SimpleActionServer<wei_goto::wei_gotoAction> as_; // NodeHandle instance must be created before this line. Otherwise strange error occurs.
  // create messages that are used to published feedback/result
  wei_goto::wei_gotoFeedback feedback_;
  wei_goto::wei_gotoResult result_;

  tf::TransformListener listener;

public:

  WEI_GOTO():
  	pub_(nh_.advertise<geometry_msgs::PoseStamped>("/move_base_simple/goal", 1000)),
    as_(nh_, "wei_goto", boost::bind(&WEI_GOTO::executeCB, this, _1), false)
  {
  	ROS_INFO("start wei_goto_server");
    as_.start();
  }
  void executeCB(const wei_goto::wei_gotoGoalConstPtr &goal)
  {
    // helper variables
    ros::Rate r(1);


    geometry_msgs::Point p = goal->target_pose.pose.position;
    tf::Quaternion gq(
          goal->target_pose.pose.orientation.x,
          goal->target_pose.pose.orientation.y,
          goal->target_pose.pose.orientation.z,
          goal->target_pose.pose.orientation.w);
    tf::Matrix3x3 m(gq);
    double roll, pitch, goal_yaw;
    m.getRPY(roll, pitch, goal_yaw);

    ROS_INFO("target position from clien :: (%lf,%lf)(%lf)",p.x,p.y,goal_yaw);

  	float distance_to_goal = 9001;
  	float yaw_diff = M_PI;
    tf::StampedTransform transform;
    transform.setOrigin( tf::Vector3(0.0, 0.0, 0.0) );
    transform.setRotation( tf::Quaternion(0, 0, 0, 1) );
    do{
    	
    	pub_.publish(goal->target_pose);


    	listener.waitForTransform("/map", "/base_link", ros::Time(0), ros::Duration(10.0) );
	    listener.lookupTransform("/map", "/base_link", ros::Time(0), transform);

		geometry_msgs::Point q;
		q.x = (float)transform.getOrigin().x(); 
		q.y = (float)transform.getOrigin().y(); 
		q.z = (float)transform.getOrigin().z();
		tf::Quaternion cq((float)transform.getRotation().x(),(float)transform.getRotation().y(),(float)transform.getRotation().z(),(float)transform.getRotation().w());
		tf::Matrix3x3 m(cq);
		double current_yaw;
		m.getRPY(roll, pitch, current_yaw);


		yaw_diff = fabs(atan2(sin(goal_yaw-current_yaw), cos(goal_yaw-current_yaw)));
		distance_to_goal = sqrt((p.x - q.x)*(p.x - q.x) + (p.y - q.y)*(p.y - q.y) + (p.z - q.z)*(p.z - q.z));


	    ROS_INFO("Current position :: (%lf,%lf)(%lf) ;; and the diff::(%lf,%lf)",q.x,q.y,current_yaw,distance_to_goal,yaw_diff);

		r.sleep();

    }while(distance_to_goal > 0.8 or yaw_diff > 0.6*M_PI);
    ROS_INFO("reach the goal::the diff (%lf,%lf)",distance_to_goal,yaw_diff);
    as_.setSucceeded();
  }


};


int main(int argc, char** argv)
{ 
  ros::init(argc, argv, "wei_goto_server");
  WEI_GOTO wei_goto1;
  ros::spin();

  return 0;
}
