//
//  OSCSkeleton.cpp
//  mvf
//
//  Created by Alejandro Van Zandt-Escobar on 26/03/2014.
//
//

#include "OSCSkeleton.h"



//--------------------------------------------------------------
void OSCSkeleton::setup(){
  
  // listen on the given port
  cout << "listening for osc messages on port " << PORT << "\n";
  receiver.setup(PORT);
  
  // Set up arrays.
  
  for (int i = 0; i < N_USERS; ++i) {
    
    for (int j = 0; j < N_JOINTS; ++j) {
      skeletons[i][j] = ofPoint(0, 0, 0);
    }
    
    centers_of_mass[i] = ofPoint(0, 0, 0);
    
    bounding_box_mins[i] = ofPoint(0, 0, 0);
    bounding_box_maxes[i] = ofPoint(0, 0, 0);
    
  }

}

//--------------------------------------------------------------
void OSCSkeleton::update(){
  
  int user_id;
  int joint_id;
  
  // check for waiting messages
  while(receiver.hasWaitingMessages()){
    // get the next message
    ofxOscMessage m;
    receiver.getNextMessage(&m);
    
    // Check for Joint Coordinates
    if(m.getAddress() == "/user/joint/coordinates"){
      // TODO: Check validity of OSC message
      
      user_id = m.getArgAsInt32(0) - 1; // Starts at 1
      joint_id = m.getArgAsInt32(1);
      
      if (user_id < N_USERS && joint_id < N_JOINTS) {
        skeletons[user_id][joint_id] = ofPoint(m.getArgAsFloat(2), m.getArgAsFloat(3), m.getArgAsFloat(4));
      }
      
    }
    else if(m.getAddress() == "/user/centerofmass"){
      // TODO: Check validity of OSC message
      
      user_id = m.getArgAsInt32(0) - 1; // Starts at 1
      
      if (user_id < N_USERS && joint_id < N_JOINTS) {
        centers_of_mass[user_id] = ofPoint(m.getArgAsFloat(1), m.getArgAsFloat(2), m.getArgAsFloat(3));
      }
      
    }
    else if(m.getAddress() == "/user/boundingbox"){
      // TODO: Check validity of OSC message
      
      user_id = m.getArgAsInt32(0) - 1; // Starts at 1
      
      if (user_id < N_USERS && joint_id < N_JOINTS) {
        bounding_box_mins[user_id] = ofPoint(m.getArgAsFloat(1), m.getArgAsFloat(2), m.getArgAsFloat(3)); // min
        bounding_box_maxes[user_id] = ofPoint(m.getArgAsFloat(4), m.getArgAsFloat(5), m.getArgAsFloat(6)); // max
      }
      
    }
    else{
      // unrecognized message: print to stderr
      string msg_string;
      msg_string = m.getAddress();
      msg_string += ": ";
      for(int i = 0; i < m.getNumArgs(); i++){
        // get the argument type
        msg_string += m.getArgTypeName(i);
        msg_string += ":";
        // display the argument - make sure we get the right type
        if(m.getArgType(i) == OFXOSC_TYPE_INT32){
          msg_string += ofToString(m.getArgAsInt32(i));
        }
        else if(m.getArgType(i) == OFXOSC_TYPE_FLOAT){
          msg_string += ofToString(m.getArgAsFloat(i));
        }
        else if(m.getArgType(i) == OFXOSC_TYPE_STRING){
          msg_string += m.getArgAsString(i);
        }
        else{
          msg_string += "unknown";
        }
      }
      fprintf(stderr, "UNRECOGNIZED MESSAGE: %s\n", msg_string.c_str());
    }
    
  }
  
}

//--------------------------------------------------------------
vector<ofPoint> OSCSkeleton::get_skeleton(int user_id) {
  
  vector<ofPoint> skeleton(N_JOINTS);
  
  for (int i = 0; i < N_JOINTS; ++i)
    skeleton[i] = skeletons[user_id][i];
  
  return skeleton;
}

//--------------------------------------------------------------
ofPoint OSCSkeleton::get_joint(int user_id, int joint_id) {
  // TODO: Validate arguments
  return skeletons[user_id][joint_id];
}


//--------------------------------------------------------------
ofPoint OSCSkeleton::get_center_of_mass(int user_id) {
  // TODO: Validate arguments
  return centers_of_mass[user_id];
}

//--------------------------------------------------------------
ofPoint OSCSkeleton::get_bounding_box_min(int user_id) {
  // TODO: Validate arguments
  return bounding_box_mins[user_id];
}

//--------------------------------------------------------------
ofPoint OSCSkeleton::get_bounding_box_max(int user_id) {
  // TODO: Validate arguments
  return bounding_box_maxes[user_id];
}





