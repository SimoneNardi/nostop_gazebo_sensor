#include "StaticTransformPublisher.h"

/////////////////////////////////////////////////
StaticTransformPublisher::StaticTransformPublisher(std::string const& model_name_, geometry_msgs::Pose const& pose_)
: ThreadBase()
, m_tf_broadcaster()
, m_model_name(model_name_)
{
      setPose(pose_);
}

/////////////////////////////////////////////////
StaticTransformPublisher::StaticTransformPublisher(std::string const& model_name_)
: ThreadBase()
, m_tf_broadcaster()
, m_model_name(model_name_)
{}

/////////////////////////////////////////////////
void StaticTransformPublisher::setPose(const geometry_msgs::Pose & pose_)
{
  // Add Static TF from /map to /robot_name/odom
      m_transform.setOrigin( tf::Vector3(pose_.position.x, pose_.position.y, pose_.position.z) );
      tf::Quaternion q;
      q.setX(pose_.orientation.x);
      q.setY(pose_.orientation.y);
      q.setZ(pose_.orientation.z);
      q.setW(pose_.orientation.w);
      
   double l_roll = atan2( 2*(pose_.orientation.x * pose_.orientation.y + pose_.orientation.z * pose_.orientation.w), 
 			     1-2*(pose_.orientation.x*pose_.orientation.x+pose_.orientation.y*pose_.orientation.y) );
   double l_pitch = asin(2*(pose_.orientation.x*pose_.orientation.z - pose_.orientation.w*pose_.orientation.y) );
   double l_yaw = atan2( 2*(pose_.orientation.x * pose_.orientation.w + pose_.orientation.y * pose_.orientation.z), 
 			    1-2*(pose_.orientation.z*pose_.orientation.z+pose_.orientation.w*pose_.orientation.w) );

   //    q.setRPY(l_roll, l_pitch, l_yaw/*+3.141593*/);
            
      m_transform.setRotation(q);
      
      ROS_INFO("---- Initialization of Static Transform Publisher /map /%s/odom: roll %lf pitch %lf yaw %lf ----", m_model_name.c_str(), l_roll, l_pitch, l_yaw);
}

/////////////////////////////////////////////////
bool StaticTransformPublisher::isSame(std::string const& model_name_)
{
  return model_name_ == m_model_name;
}

/////////////////////////////////////////////////
void StaticTransformPublisher::run()
{
  ros::Rate loop_rate(10);

  while (ros::ok())
  {
    m_tf_broadcaster.sendTransform(tf::StampedTransform(m_transform, ros::Time::now(), "/map", "/"+m_model_name+"/odom"));
    
    ros::spinOnce();
    
    loop_rate.sleep();
  }
}