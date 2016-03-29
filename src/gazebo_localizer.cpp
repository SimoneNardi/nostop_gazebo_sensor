#include "ros/ros.h"

#include <gazebo_msgs/GetModelState.h>

#include <cstdlib>

#include "StaticTransformPublisher.h"

int main(int argc, char **argv)
{
      ros::init(argc, argv, "gazebo_loc");
                  
      // Info From Launch File
      
      // Identify robot name:
      std::string l_name;
      ros::NodeHandle l_node("~");
      
      std::string temp_name;
      if ( l_node.searchParam("robot_name", temp_name) )
      {
	  l_node.getParam(temp_name,l_name);
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
            
      ros::Rate loop_rate(10);
          
      bool l_static_transform_initialized = false;
      
      geometry_msgs::Pose l_start;
      l_start.position.x = 0;
      l_start.position.y = 0;
      l_start.position.z = 0;
      
      l_start.orientation.x = 0;
      l_start.orientation.y = 0;
      l_start.orientation.z = 0;
      l_start.orientation.w = 1;
            
      StaticTransformPublisher l_tf(l_getmodelstate.request.model_name);
      
      int count = 0;
      while (ros::ok())
      {
	  if( l_clientGet.call(l_getmodelstate) )
	  {
	    if (l_getmodelstate.response.success)
	    {
	      if(!l_static_transform_initialized)
	      {
		l_tf.setPose(l_start);
		//l_tf.setPose(l_getmodelstate.response.pose);
		l_tf.start();
		l_static_transform_initialized = true;
	      }
	      
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
	    ROS_ERROR("Failed to call service");
	    return -1;
	  }
	  	  
	  ros::spinOnce();
	  
	  loop_rate.sleep();
	  ++count;
      }
      
      l_tf.stop();
 }