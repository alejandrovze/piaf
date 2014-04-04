//
//  KinectInput.cpp
//  mvf
//
//  Created by Alejandro Van Zandt-Escobar on 03/04/2014.
//
//

#include "KinectInput.h"


KinectInput* KinectInput::ms_self = NULL;


KinectInput::KinectInput()
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
  return openni::STATUS_OK;
  
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
      // Joints
      
      for (nite::JointType joint = nite::JOINT_HEAD; joint < nite::JOINT_RIGHT_FOOT; joint++) {
        
        const nite::Point3f joint_pos = user.getSkeleton().getJoint(joint).getPosition();
        
        data_point.joints.push_back(ofPoint(joint_pos.x, joint_pos.y, joint_pos.z));
        
      }
      
      // Center of Mass
      
      data_point.center_of_mass = ofPoint(user.getCenterOfMass().x, user.getCenterOfMass().y, user.getCenterOfMass().z);
      
      data_point.bounding_box_min = ofPoint(user.getBoundingBox().min.x, user.getBoundingBox().min.y, user.getBoundingBox().min.z);
      
      data_point.bounding_box_max = ofPoint(user.getBoundingBox().max.x, user.getBoundingBox().max.y, user.getBoundingBox().max.z);
    }
  }

  return data_point;
  
}


// TODO: Get State
const nite::UserData& KinectInput::get_user(int user_id) {
  
  // Update Data
  nite::UserTrackerFrameRef userTrackerFrame;
  
	nite::Status rc = user_tracker->readFrame(&userTrackerFrame);
  
	if (rc != nite::STATUS_OK)
	{
		printf("GetNextData failed\n");
		return;
	}
  
	const nite::Array<nite::UserData>& users = userTrackerFrame.getUsers();

  
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
  
  return user;

}
          
          
