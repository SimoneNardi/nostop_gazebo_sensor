#ifndef STATIC_TRANSFORM_PUBLISHER_H
#define STATIC_TRANSFORM_PUBLISHER_H
#pragma once

#include "ThreadBase.h"

#include <tf/transform_broadcaster.h>

class StaticTransformPublisher : public Robotics::GameTheory::ThreadBase
{
protected:
  tf::TransformBroadcaster m_tf_broadcaster;
  tf::Transform m_transform;
  std::string m_model_name;
public:
  StaticTransformPublisher(std::string const& model_name);
  StaticTransformPublisher(std::string const& model_name, geometry_msgs::Pose const& pose_);
  
  virtual void run();
  
  bool isSame(std::string const& model_name_);
  
  void setPose(const geometry_msgs::Pose &pose_);
};

#endif
