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
#include "ofMain.h"


// ------------------------------------------------------
// ------------------------------------------------------
// Skeleton Gesture Structure
// ------------------------------------------------------
// ------------------------------------------------------


typedef struct SkeletonDataPoint {
  
  // Constructors
  SkeletonDataPoint():
  joints(N_JOINTS),
  center_of_mass(ofPoint(0, 0, 0)),
  bounding_box_min(ofPoint(0, 0, 0)),
  bounding_box_max(ofPoint(0, 0, 0))
  {
  }
  
  SkeletonDataPoint(vector<ofPoint> _joints,
                    ofPoint _center_of_mass,
                    ofPoint _bounding_box_min,
                    ofPoint _bounding_box_max):
  joints(N_JOINTS),
  center_of_mass(_center_of_mass),
  bounding_box_min(_bounding_box_min),
  bounding_box_max(_bounding_box_max)
  {
    for (int i = 0; i < N_JOINTS; ++i)
      joints[i] = _joints[i];
  }
  
  ~SkeletonDataPoint()
  {
    vector<ofPoint> blank;
    joints.swap(blank);
  }
  
  // Skeleton data
  vector<ofPoint> joints;
  
} SkeletonDataPoint;

typedef struct SkeletonGesture {
  
  SkeletonGesture(int _id, string _name):
  id(_id),
  name(_name)
  {
  }
  
  ~SkeletonGesture()
  {
    vector<SkeletonDataPoint> blank;
    data.swap(blank);
  }
  
  void add_data(vector<ofPoint> _joints, ofPoint _center_of_mass, ofPoint _bounding_box_min,
                ofPoint _bounding_box_max)
  {
    data.push_back(SkeletonDataPoint());
    
//    data.end
//    _joints, _center_of_mass,
//                                     _bounding_box_min, _bounding_box_max));
  }
  
  int get_length()
  {
    return data.size();
  }
  
  // Metadata
  int id;
  string name;
  
  // Data
  vector<vector<ofPoint> data;
  vector<ofPoint> center_of_mass;
  vector<ofPoint> bounding_box_min;
  vector<ofPoint> bounding_box_max;
  
} SkeletonGesture;


// ------------------------------------------------------
// ------------------------------------------------------
// GVFKINECT CLASS
// ------------------------------------------------------
// ------------------------------------------------------

class gvfKinect {
  
public:
  
  gvfKinect();
//  ~gvfKinect(); TODO
  
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
  
  void saveGestures(string filename);
  void loadGestures(string filename);
  
  // GETS
  ofxGVF::ofxGVFState get_state();
  bool get_is_playing();
  
  int get_n_templates();
  
private:
  
  gvfKinectHandler* gvfh;
  
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
  
};

#endif /* defined(__mvf__gvfKinect__) */
