//
//  KinectInput.cpp
//  mvf
//
//  Created by Alejandro Van Zandt-Escobar on 03/04/2014.
//
//

#include "KinectInput.h"


KinectInput* KinectInput::ms_self = NULL;


KinectInput::KinectInput():
is_running(false)
{
	ms_self = this;
  
	user_tracker = new nite::UserTracker;
  
  for (int i = 0; i < MAX_USERS; ++i) {
    user_states[i] = nite::SKELETON_NONE;
    users_lost[i] = false;
    users_visible[i] = false;
  }
  
}

KinectInput::~KinectInput()
{
  close();
  
	ms_self = NULL;
}

void KinectInput::close()
{
	delete user_tracker;
	nite::NiTE::shutdown();
	openni::OpenNI::shutdown();
}

openni::Status KinectInput::setup()
{
  
  // Initialize openNI
	openni::Status rc = openni::OpenNI::initialize();
	if (rc != openni::STATUS_OK)
	{
		printf("Failed to initialize OpenNI\n%s\n", openni::OpenNI::getExtendedError());
		return rc;
	}
  
  // Open Device (Kinect)
	const char* deviceUri = openni::ANY_DEVICE;
  
	rc = device.open(deviceUri);
  
	if (rc != openni::STATUS_OK)
	{
		printf("Failed to open device\n%s\n", openni::OpenNI::getExtendedError());
		return rc;
	}
  
  // Initialize NiTE
	nite::NiTE::initialize();
  
	if (user_tracker->create(&device) != nite::STATUS_OK)
	{
		return openni::STATUS_ERROR;
	}
  
  // If all else has succeeded.
  
  is_running = true;
  
  return openni::STATUS_OK;
  
}

void KinectInput::update() {
  
  nite::Status rc = user_tracker->readFrame(&userTrackerFrame);
  
	if (rc != nite::STATUS_OK)
	{
		printf("GetNextData failed\n");
		return;
	}
  const nite::Array<nite::UserData>& users = userTrackerFrame.getUsers();
  
  int user_id = 0; // Late generalize?
  
  if (user_id >= users.getSize()) {
    // Error
    return;
  }
  
  const nite::UserData& user = users[user_id];
  
  // Print information about user state.
  {
    if (user.isNew())
    {
      fprintf(stderr, "User %d: New\n", user_id);
    }
    else if (users_visible[user_id] != user.isVisible()) {
      if ((users_visible[user_id] = user.isVisible())) {
        fprintf(stderr, "User %d: Visible user\n", user_id);
      }
    }
    else if (users_lost[user_id] != user.isLost()) {
      if ((users_lost[user_id] = user.isLost())) {
        fprintf(stderr, "User %d: Lost user\n", user_id);
      }
    }
    
    if (user_states[user_id] != user.getSkeleton().getState()) {
      switch(user_states[user_id] = user.getSkeleton().getState()) // Set in array
      {
        case nite::SKELETON_NONE:
          fprintf(stderr, "User %d: Stopped tracking.\n", user_id);
          break;
        case nite::SKELETON_CALIBRATING:
          fprintf(stderr, "User %d: Calibrating.\n", user_id);
          break;
        case nite::SKELETON_TRACKED:
          fprintf(stderr, "User %d: Tracking.\n", user_id);
          break;
        case nite::SKELETON_CALIBRATION_ERROR_NOT_IN_POSE:
        case nite::SKELETON_CALIBRATION_ERROR_HANDS:
        case nite::SKELETON_CALIBRATION_ERROR_LEGS:
        case nite::SKELETON_CALIBRATION_ERROR_HEAD:
        case nite::SKELETON_CALIBRATION_ERROR_TORSO:
          fprintf(stderr, "User %d: Calibration failed.\n", user_id);
          break;
      }
    }
  }
}

SkeletonDataPoint KinectInput::get_data(int user_id) {
  
  SkeletonDataPoint data_point;
  
  const nite::UserData& user = get_user(user_id);
  
  nite::UserTracker* pUserTracker;
    
  if (user.isNew())
  {
    user_tracker->startSkeletonTracking(user.getId());
  }
  else if (!user.isLost())
  {
    
    if (user.getSkeleton().getState() == nite::SKELETON_TRACKED)
    {
      
      // Center of Mass
      
      ofPoint com = ofPoint(user.getCenterOfMass().x, user.getCenterOfMass().y, user.getCenterOfMass().z);
      
      data_point.center_of_mass = com;
      
      // Joints
      
      for (nite::JointType joint = nite::JOINT_HEAD; joint < nite::JOINT_RIGHT_FOOT; joint++) {
        
        const nite::Point3f joint_pos = user.getSkeleton().getJoint(joint).getPosition();
        
        // Joint position relative to center of mass.
        data_point.joints.at(joint) = (com - ofPoint(joint_pos.x, joint_pos.y, joint_pos.z));
        data_point.confidences.at(joint) = user.getSkeleton().getJoint(joint).getPositionConfidence();
        
        // Joint orientations.
        
        // Convert from NiteQuaternion to ofQuaternion
        NiteQuaternion orientation = user.getSkeleton().getJoint(joint).getOrientation();
        data_point.joint_orientations.at(joint) = ofQuaternion(orientation.x, orientation.y, orientation.z, orientation.w);
        data_point.orientation_confidences.at(joint) = user.getSkeleton().getJoint(joint).getOrientationConfidence();
        
      }
      
      // TODO: bounding box
      
      data_point.bounding_box_min = ofPoint(user.getBoundingBox().min.x, user.getBoundingBox().min.y, user.getBoundingBox().min.z);
      
      data_point.bounding_box_max = ofPoint(user.getBoundingBox().max.x, user.getBoundingBox().max.y, user.getBoundingBox().max.z);
    }
  }

  return data_point;
  
}

SkeletonDataPoint KinectInput::get_depth_data(int user_id) {
  
  SkeletonDataPoint depth_data_point;
  
  const nite::UserData& user = get_user(user_id);
  
  nite::UserTracker* pUserTracker;
  
  if (user.isNew())
  {
    user_tracker->startSkeletonTracking(user.getId());
  }
  else if (!user.isLost())
  {
    
    if (user.getSkeleton().getState() == nite::SKELETON_TRACKED)
    {
      // Joints
      
      for (nite::JointType joint = nite::JOINT_HEAD; joint <= nite::JOINT_RIGHT_FOOT; joint++) {
        
        const nite::Point3f joint_pos = user.getSkeleton().getJoint(joint).getPosition();
        
        const float joint_conf = user.getSkeleton().getJoint(joint).getPositionConfidence();
        
        depth_data_point.joints.at(joint) = convert_world_to_depth(ofPoint(joint_pos.x, joint_pos.y, joint_pos.z));
        depth_data_point.confidences.at(joint) = joint_conf;
        
      }
      
      // Center of Mass
      
      depth_data_point.center_of_mass = convert_world_to_depth(ofPoint(user.getCenterOfMass().x, user.getCenterOfMass().y, user.getCenterOfMass().z));
      
      depth_data_point.bounding_box_min = convert_world_to_depth(ofPoint(user.getBoundingBox().min.x, user.getBoundingBox().min.y, user.getBoundingBox().min.z));
      
      depth_data_point.bounding_box_max = convert_world_to_depth(ofPoint(user.getBoundingBox().max.x, user.getBoundingBox().max.y, user.getBoundingBox().max.z));
    }
  }
  
  return depth_data_point;
}


const nite::UserData& KinectInput::get_user(int user_id) {
  
  
	const nite::Array<nite::UserData>& users = userTrackerFrame.getUsers();
  
  if (user_id >= users.getSize()) {
    // Error
    return;
  }
  
  const nite::UserData& user = users[user_id];
  
  return user;

}

nite::SkeletonState KinectInput::get_state(int user_id) {
  
  return user_states[user_id];
  
}





openni::VideoFrameRef KinectInput::get_depth_frame() {

  return userTrackerFrame.getDepthFrame();
  
}


ofPoint KinectInput::convert_world_to_depth(ofPoint coordinates) {
  
  float x;
  float y;
  
  user_tracker->convertJointCoordinatesToDepth(coordinates.x, coordinates.y, coordinates.z, &x, &y);
  
  return ofPoint(x,y);
  
}

bool KinectInput::get_is_running() {
  return is_running;
}

          
