//
//  KinectInput.h
//  mvf
//
//  Created by Alejandro Van Zandt-Escobar on 03/04/2014.
//
//

#ifndef __mvf__KinectInput__
#define __mvf__KinectInput__

#include <iostream>

#include "NiTE.h"

#include "SkeletonGesture.h"


#define MAX_USERS 1

class KinectInput
{

public:
  
	KinectInput();
	virtual ~KinectInput();
  
	virtual openni::Status setup();
  void close();
  void update();
  
  SkeletonDataPoint get_data(int user_id = 0);
  const nite::UserData& get_user(int user_id = 0);
  openni::VideoFrameRef get_depth_frame();
  
  nite::SkeletonState get_state(int user_id = 0);
  
  ofPoint convert_world_to_depth(ofPoint coordinates);
  
  SkeletonDataPoint get_depth_data(int user_id = 0); // For display
  
  bool get_is_running();
  
private:
  
	static KinectInput* ms_self;
  
	openni::Device device;
	nite::UserTracker* user_tracker;
  nite::UserTrackerFrameRef userTrackerFrame;
  
  nite::SkeletonState user_states[MAX_USERS];
  bool users_lost[MAX_USERS];
  bool users_visible[MAX_USERS];
  
  bool is_running;
  
};


#endif /* defined(__mvf__KinectInput__) */
