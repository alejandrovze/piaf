//
//  gvfKinect.h
//  mvf
//
//  Created by Alejandro Van Zandt-Escobar on 28/03/2014.
//


#ifndef __mvf__gvfKinect__
#define __mvf__gvfKinect__

#include <iostream>

#include "gvfKinectHandler.h"
#include "OSCSkeleton.h"
#include "SkeletonGesture.h"

#include "ofxXmlSettings.h"

// ------------------------------------------------------
// ------------------------------------------------------
// GVFKINECT CLASS
// ------------------------------------------------------
// ------------------------------------------------------

class gvfKinect {
  
public:
  
  gvfKinect();
  ~gvfKinect();
  
  void setup();
  void exit();
  
  void update();
  void update(SkeletonDataPoint data_point);
  
  void draw();
  
  // Set State
  void learn();
  void follow();
  void clear();
  
  // Accept or Deny Input
  void play();
  void stop();
  
  // LOAD / SAVE
  void saveGestures();
  void saveGestures(string filename);
  void loadGestures();
  void loadGestures(string filename);
  
  // GETS
  ofxGVF::ofxGVFState get_state();
  bool get_is_playing();
  int get_n_templates();
  
private:
  
  // State
  ofxGVF::ofxGVFState state;
  bool is_playing;
  
  // Input and gestures;
  OSCSkeleton skeleton_input;
  vector<SkeletonGesture> skeleton_templates;
  SkeletonGesture current_gesture;
  int addTemplate();
  int current_template;
  // TODO: addTemplates with specific metadata
  
  // GVF Proccesing
  
  vector<gvfKinectHandler*> gvf_handlers;
  
  void gvf_input(SkeletonDataPoint data_point);
  
  
};

#endif /* defined(__mvf__gvfKinect__) */
