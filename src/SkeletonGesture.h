//
//  SkeletonGesture.h
//  mvf
//
//  Created by Alejandro Van Zandt-Escobar on 30/03/2014.
//
//

#ifndef __mvf__SkeletonGesture__
#define __mvf__SkeletonGesture__

#include <iostream>
#include "ofMain.h"

typedef enum
{
  GESTURE_TEMPLATE,
  GESTURE_RECORD
  
} GestureType;

#define N_USERS 1
#define N_JOINTS 15

// ------------------------------------------------------
// ------------------------------------------------------
// Skeleton Gesture Structure
// ------------------------------------------------------
// ------------------------------------------------------


typedef struct SkeletonDataPoint {

  // Constructors
  SkeletonDataPoint():
  joints(N_JOINTS),
  confidences(N_JOINTS),
  joint_orientations(N_JOINTS),
  orientation_confidences(N_JOINTS),
  center_of_mass(ofPoint(0, 0, 0)),
  bounding_box_min(ofPoint(0, 0, 0)),
  bounding_box_max(ofPoint(0, 0, 0))
  {
  }

  SkeletonDataPoint(vector<ofPoint> _joints,
                    vector<float> _confidences,
                    vector<ofQuaternion> _joint_orientations,
                    vector<float> _orientation_confidences,
                    ofPoint _center_of_mass,
                    ofPoint _bounding_box_min,
                    ofPoint _bounding_box_max):
  joints(N_JOINTS),
  confidences(N_JOINTS),
  joint_orientations(N_JOINTS),
  orientation_confidences(N_JOINTS),
  center_of_mass(_center_of_mass),
  bounding_box_min(_bounding_box_min),
  bounding_box_max(_bounding_box_max)
  {
    for (int i = 0; i < N_JOINTS; ++i) {
      joints[i] = _joints[i];
      confidences[i] = _confidences[i];
      joint_orientations[i] = _joint_orientations[i];
      orientation_confidences[i] = _orientation_confidences[i];
    }
  }

  ~SkeletonDataPoint()
  {
    joints.clear();
    confidences.clear();
    joint_orientations.clear();
    orientation_confidences.clear();
  }

  // Skeleton data
  vector<ofPoint> joints;
  vector<float> confidences;
  vector<ofQuaternion> joint_orientations;
  vector<float> orientation_confidences;
  ofPoint center_of_mass;
  ofPoint bounding_box_min;
  ofPoint bounding_box_max;
  
  float timing;

} SkeletonDataPoint;


class SkeletonGesture {
  
public:
  
    SkeletonGesture(int _id, string _name):
    id(_id),
    name(_name)
    {
    }
    
    ~SkeletonGesture()
    {
      data.clear();
    }
  
    void add_data(SkeletonDataPoint data_point)
    {
      data.push_back(data_point);
    }
    
    int get_length()
    {
      return data.size();
    }
    void clear()
    {
      data.clear();
    }
    int get_id() {
      return id;
    }
    string get_name() {
      return name;
    }
    SkeletonDataPoint get_data_point(int index) {
      return data[index];
    }
  
private:
  
  // Metadata
  int id;
  string name;
  
  // Data
  vector<SkeletonDataPoint> data;
  
};

#endif /* defined(__mvf__SkeletonGesture__) */
