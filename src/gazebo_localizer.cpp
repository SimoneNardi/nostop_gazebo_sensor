#include "ros/ros.h"

#include <gazebo_msgs/GetModelState.h>

#include <cstdlib>

int main(int argc, char **argv)
{
      ros::init(argc, argv, "gazebo_loc");
                  
      // Info From Launch File
      
      // Identify robot name:
      std::string l_name;
      ros::NodeHandle l_node("~");
      if ( l_node.getParam("robot_name", l_name) )
      {
	  ROS_INFO("Gazebo Localizer Nome ricevuto: %s", l_name.c_str());
      }
      else
      {
	  std::cout << "Enter the name of the robot: ";
	  //std::cin >> l_name;
	  l_name="red_blue";
	  ROS_ERROR("Nome non ricevuto: %s", l_name.c_str());
      }
      
      ///
      std::string l_pub_name = "/";
      l_pub_name += l_name;
      l_pub_name += "/localizer/gazebo/pose";
      
      ros::Publisher l_posePub = l_node.advertise<geometry_msgs::Pose>(l_pub_name.c_str(), 10);
      ros::ServiceClient l_clientGet = l_node.serviceClient<gazebo_msgs::GetModelState>("/gazebo/get_model_state"); 
      gazebo_msgs::GetModelState l_getmodelstate;
      l_getmodelstate.request.model_name = l_name;
            
      ros::Rate loop_rate(5);
    
      int count = 0;
      while (ros::ok())
      {
	  if( l_clientGet.call(l_getmodelstate) )
	  {
	    if (l_getmodelstate.response.success)
	    {
	      l_posePub.publish<geometry_msgs::Pose>(l_getmodelstate.response.pose);
	      ROS_DEBUG("Publish postion of %s: [%f, %f ,%f]", l_name.c_str(), 
		       (double)l_getmodelstate.response.pose.position.x, 
		       (double)l_getmodelstate.response.pose.position.y, 
		       (double)l_getmodelstate.response.pose.position.z);
	    }
	    else
	      ROS_INFO("Model %s not found!", l_name.c_str());
	  }
	  else
	  {
	    ROS_ERROR("Failed to call service ");
	    return -1;
	  }
	  	  
	  ros::spinOnce();
	  
	  loop_rate.sleep();
	  ++count;
      }
 }