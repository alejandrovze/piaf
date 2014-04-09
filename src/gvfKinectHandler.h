//
//  gvfKinectHandler.h
//  mvf
//
//  Created by Alejandro Van Zandt-Escobar on 28/03/2014.
//
//

#ifndef __mvf__gvfKinectHandler__
#define __mvf__gvfKinectHandler__

#include <iostream>
#include "ofMain.h"

#include "ofxGVF.h"

#include "SkeletonGesture.h"

// 

typedef enum
{
  CENTER_OF_MASS,
  
  LEFT_ELBOW,
  RIGHT_ELBOW,
  
  LEFT_KNEE,
  RIGHT_KNEE,
  
  RIGHT_ARM,
  LEFT_ARM,
  RIGHT_LEG,
  LEFT_LEG
  
} KinectFeature;

// Rather than a vector, struct to hold estimated status for a given vector.
typedef struct {
    float likelihoods;
    float probability;
    float phase;
    float speed;
    float scale;
    float rotation;
} RecognitionInfo;

class gvfKinectHandler{
  
public:

  gvfKinectHandler(int _gvf_id, string _gvf_name, KinectFeature feature);
  ~gvfKinectHandler();
  
  void setState(ofxGVF::ofxGVFState state);
  void addTemplate();
  void gvf_data(SkeletonDataPoint data_point);
  void gvf_data(ofPoint p); // Kinect Input is 3D point.
  
  // Metadata
  int gvf_id;
  string gvf_name;
  
  // SET parameters
  void setSmoothingCoefficient(float smoothingCoeficient);
  void setResamplingThreshold(int resamplingThreshold);
  void setAdaptSpeed(ofxGVF::ofxGVFVarianceCoefficents varianceCoefficients);
  void setNumberOfParticles(int numParticles);
  
  ofxGVF::ofxGVFState getState();

  // amount of templates
  int getTemplateCount();
  
  // returns the index of the template that is, most probably,
  // the one the user is trying to perform
  int getIndexMostProbable();
  RecognitionInfo getRecogInfoOfMostProbable();
  RecognitionInfo getRecogInfo(int template_id);
  
  
private:
  
  ofxGVF *mygvf;
  
  KinectFeature gvf_feature; // Feature studied
  
  // FEATURE EXTRACTION
  ofVec3f get_segment(ofPoint joint1, ofPoint joint2);
  float get_angle(ofVec3f segment1, ofVec3f segment2); // Angle between two arm segments. 
  
};

#endif /* defined(__mvf__gvfKinectHandler__) */
