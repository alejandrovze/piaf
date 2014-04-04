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

#include "NiteSampleUtilities.h"


#define MAX_USERS 1

class KinectInput
{

public:
  
	KinectInput();
	virtual ~KinectInput();
  
	virtual openni::Status setup();
  void close();
  
  SkeletonDataPoint get_data(int user_id = 0);
  const nite::UserData& get_user(int user_id = 0); // TODO
  
private:
  
	static KinectInput* ms_self;
  
	openni::Device device;
	nite::UserTracker* user_tracker;
  
  nite::SkeletonState user_states[MAX_USERS];
  bool users_lost[MAX_USERS];
  bool users_visible[MAX_USERS];
  
};


#endif /* defined(__mvf__KinectInput__) */
