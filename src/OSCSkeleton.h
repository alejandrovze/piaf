//
//  OSCSkeleton.h
//  mvf
//
//  Created by Alejandro Van Zandt-Escobar on 26/03/2014.
//
//

#ifndef __mvf__OSCSkeleton__
#define __mvf__OSCSkeleton__

#include <iostream>
#include "ofxOsc.h"

#define PORT 7000
#define NUM_MSG_STRINGS 20
#define N_USERS 1
#define N_JOINTS 15

class OSCSkeleton {
  
public:

  void setup();
  void update();
  
  vector<ofPoint> get_skeleton(int user_id);
  ofPoint get_joint(int user_id, int joint_id);
  ofPoint get_center_of_mass(int user_id);
  ofPoint get_bounding_box_min(int user_id);
  ofPoint get_bounding_box_max(int user_id);
  
private:
  
  ofPoint skeletons[N_USERS][N_JOINTS];
  ofPoint centers_of_mass[N_USERS];
  ofPoint bounding_box_mins[N_USERS];
  ofPoint bounding_box_maxes[N_USERS];
  
  ofxOscReceiver receiver;
  
};


#endif /* defined(__mvf__OSCSkeleton__) */
