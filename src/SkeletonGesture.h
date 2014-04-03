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
    joints.clear();
  }

  // Skeleton data
  vector<ofPoint> joints;
  ofPoint center_of_mass;
  ofPoint bounding_box_min;
  ofPoint bounding_box_max;

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
