#include <ros/ros.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include <wei_goto/wei_gotoAction.h>

int main (int argc, char **argv)
{
  ros::init(argc, argv, "wei_goto_client");

  // create the action client
  // true causes the client to spin its own thread
  actionlib::SimpleActionClient<wei_goto::wei_gotoAction> ac("wei_goto", true);

  ROS_INFO("Waiting for action server to start.");
  // wait for the action server to start
  ac.waitForServer(); //will wait for infinite time

  ROS_INFO("Action server started, sending goal.");
  // send a goal to the action
  wei_goto::wei_gotoGoal goal;
  goal.target_pose.header.frame_id = "map";
  goal.target_pose.pose.position.x = 2;
  goal.target_pose.pose.position.y = 0;
  goal.target_pose.pose.position.z = 0;
  goal.target_pose.pose.orientation.x = 0; 
  goal.target_pose.pose.orientation.y = 0;
  goal.target_pose.pose.orientation.z = 0;
  goal.target_pose.pose.orientation.w =  1;
  ROS_INFO("client goal:: (%lf,%lf)(%lf,%lf,%lf,%lf)",
    goal.target_pose.pose.position.x ,
    goal.target_pose.pose.position.y ,
    goal.target_pose.pose.orientation.x , 
    goal.target_pose.pose.orientation.y ,
    goal.target_pose.pose.orientation.z ,
    goal.target_pose.pose.orientation.w 
  );

  ac.sendGoal(goal);

  ac.waitForResult();

  if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED){
    ROS_INFO("reached the goal");
  }
  else {
    ROS_INFO("Failed to reach the goal");
  }

  // //wait for the action to return
  // bool finished_before_timeout = ac.waitForResult(ros::Duration(30.0));

  // if (finished_before_timeout)
  // {
  //   actionlib::SimpleClientGoalState state = ac.getState();
  //   ROS_INFO("Action finished: %s",state.toString().c_str());
  //   wei_goto::wei_gotoResult result = *(ac.getResult());
  //   std::stringstream result_string;
  //   std::copy(result.sequence.begin(), result.sequence.end(),
  //             std::ostream_iterator<int>(result_string, " "));
  //   ROS_INFO("Action result: %s", result_string.str().c_str());
  // }
  // else
  //   ROS_INFO("Action did not finish before the time out.");

  //exit
  return 0;
}
