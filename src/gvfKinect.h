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
#include "SkeletonGesture.h"
#include "KinectInput.h"

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
  
  // Set State
  void learn();
  void follow();
  void clear();
  
  // Accept or Deny Input
  void play();
  void stop();
  
  // Set to live or recorded input
  void set_live(bool _is_live);
  
  
  // SETS
  void set_current_record(int _current_record);
  
  // GETS
  ofxGVF::ofxGVFState get_state();
  bool get_is_playing();
  bool get_is_live();
  int get_n_templates();
  int get_n_records();
  int get_current_record();
  SkeletonDataPoint get_current_point();
  SkeletonDataPoint get_depth_data_point(); // For drawing
  
  openni::VideoFrameRef get_depth_frame();
  
  // LOAD / SAVE
  void saveGestures();
  void saveGestures(string filename);
  void loadGestures();
  void loadGestures(string filename);
  
private:
  
  // State
  ofxGVF::ofxGVFState state;
  bool is_playing;
  bool is_live;
  
  // ---------------------------------------
  
  // Kinect Input
  
  KinectInput kinect_input;
  
  SkeletonDataPoint current_point;
  
  vector<SkeletonGesture> skeleton_templates;
  int add_template();
  int current_template;
  
  vector<SkeletonGesture> skeleton_gesture_records;
  int add_record();
  int current_record;
  int playback_position;
  
  // ---------------------------------------
  
  // GVF Proccesing
  
  vector<gvfKinectHandler*> gvf_handlers;
  void gvf_input(SkeletonDataPoint data_point);
  
  // ---------------------------------------
  
  void writeGesture(ofxXmlSettings* gesture_file, SkeletonGesture* saved_gesture, int id, string type);
  void loadGesture(ofxXmlSettings* gesture_file, int id, string type);
  
};

#endif /* defined(__mvf__gvfKinect__) */
