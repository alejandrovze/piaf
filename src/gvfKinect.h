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
#include "ofxCsv.h"

// FOR LOADING MS KINECT CSV FILES

typedef enum {
  NUI_SKELETON_POSITION_HIP_CENTER = 0,
  NUI_SKELETON_POSITION_SPINE = 1,
  NUI_SKELETON_POSITION_SHOULDER_CENTER = 2,
  NUI_SKELETON_POSITION_HEAD = 3,
  NUI_SKELETON_POSITION_SHOULDER_LEFT = 4,
  NUI_SKELETON_POSITION_ELBOW_LEFT = 5,
  NUI_SKELETON_POSITION_WRIST_LEFT = 6,
  NUI_SKELETON_POSITION_HAND_LEFT = 7,
  NUI_SKELETON_POSITION_SHOULDER_RIGHT = 8,
  NUI_SKELETON_POSITION_ELBOW_RIGHT = 9,
  NUI_SKELETON_POSITION_WRIST_RIGHT = 10,
  NUI_SKELETON_POSITION_HAND_RIGHT = 11,
  NUI_SKELETON_POSITION_HIP_LEFT = 12,
  NUI_SKELETON_POSITION_KNEE_LEFT = 13,
  NUI_SKELETON_POSITION_ANKLE_LEFT = 14,
  NUI_SKELETON_POSITION_FOOT_LEFT = 15,
  NUI_SKELETON_POSITION_HIP_RIGHT = 16,
  NUI_SKELETON_POSITION_KNEE_RIGHT = 17,
  NUI_SKELETON_POSITION_ANKLE_RIGHT = 18,
  NUI_SKELETON_POSITION_FOOT_RIGHT = 19,

  NUI_SKELETON_POSITION_COUNT = 20
  
} MSKinectJoints;

typedef struct {
  
  int n_gvf;
  
  vector<string> gvf_names;
  vector<int> most_probable_gesture;
  vector<RecognitionInfo> recognition_info; // For each GVF
  
} RecognitionData;


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
  
  // Set State
  void learn();
  void follow();
  void clear();
  
  // Accept or Deny Input
  void play();
  void stop();
  
  
  // SETS
  void set_current_record(int _current_record);
  void set_live(bool _is_live); // Set to live or recorded input
  // ------------------------------------
  
  // GETS
  ofxGVF::ofxGVFState get_state();
  bool get_is_playing();
  bool get_is_live();
  SkeletonDataPoint get_current_point();
  SkeletonDataPoint get_depth_data_point(); // For drawing
  
  openni::VideoFrameRef get_depth_frame();
  string get_tracking_state();
  
  int get_n_templates();
  
  int get_n_records();
  int get_current_record();
  int get_playback_position();
  int get_current_record_length();
  
  RecognitionData get_recognition_data();
  int get_most_probable();
  int get_most_probable(int gvf_index);
  int get_n_gvfs();
  string get_gvf_name(int gvf_index);
  
  SkeletonGesture* get_template(int template_index);
  // ------------------------------------
  
  // LOAD / SAVE
  void saveGestures();
  void saveGestures(string filename);
  void loadGestures();
  void loadGestures(string filename);
  void loadCsv();
  
  void delete_template(int template_index); // TODO
  // ------------------------------------
  
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
  
  void update(SkeletonDataPoint data_point);
  
  // ---------------------------------------
  
  // GVF Proccesing
  
  vector<gvfKinectHandler*> gvf_handlers;
  void gvf_input(SkeletonDataPoint data_point);
  
  void update_status();
  
  // ---------------------------------------
  
  void writeGesture(ofxXmlSettings* gesture_file, SkeletonGesture* saved_gesture, int id, string type);
  void loadGesture(ofxXmlSettings* gesture_file, int id, string type);
  
};

#endif /* defined(__mvf__gvfKinect__) */
