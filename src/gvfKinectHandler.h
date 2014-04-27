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

// 17 BODY PARTS
typedef enum
{
  
  // LEVEL 0
  BODY,
  
  // LEVEL 1
  UPPER_BODY,
  LOWER_BODY,
  
  // LEVEL 2
  // - UPPER_BODY
  LEFT_ARM,
  RIGHT_ARM,
  BUST,
  
  // - LOWER_BODY
  LEFT_LEG,
  RIGHT_LEG,
  WAIST,
  
  // LEVEL 3
  // - UPPER_BODY
  // --- LEFT_ARM
  LEFT_SHOULDER,
  LEFT_ELBOW,
  
  // --- RIGHT_ARM
  RIGHT_SHOULDER,
  RIGHT_ELBOW,
  
  // --- BUST
  HEAD,
  NECK,
  
  // - LOWER_BODY
  // --- LEFT_LEG
  LEFT_HIP,
  LEFT_KNEE,
  
  // --- RIGHT_LEG
  RIGHT_HIP,
  RIGHT_KNEE,
  
  // --- WAIST
  
  // ------------------------------
  // LEVEL 4 - extremities
  LEFT_HAND,
  RIGHT_HAND
  
} BodyPart;

typedef enum
{
	JOINT_HEAD,
	JOINT_NECK,
  
	JOINT_LEFT_SHOULDER,
	JOINT_RIGHT_SHOULDER,
	JOINT_LEFT_ELBOW,
	JOINT_RIGHT_ELBOW,
	JOINT_LEFT_HAND,
	JOINT_RIGHT_HAND,
  
	JOINT_TORSO,
  
	JOINT_LEFT_HIP,
	JOINT_RIGHT_HIP,
	JOINT_LEFT_KNEE,
	JOINT_RIGHT_KNEE,
	JOINT_LEFT_FOOT,
	JOINT_RIGHT_FOOT,
} JointType;


// Rather than a vector, struct to hold estimated status for a given vector.
typedef struct {
  
    float probability;
    float phase;
    float speed;
    float scale;
    float rotation;
  
} RecognitionInfo;

class gvfKinectHandler{
  
public:

  gvfKinectHandler(string _gvf_name, BodyPart feature, int n_dimensions);
  ~gvfKinectHandler();
  
  void setState(ofxGVF::ofxGVFState state);
  void addTemplate();
  void gvf_data(SkeletonDataPoint data_point);
  void gvf_data(ofPoint p); // Kinect Input is 3D point.
  void gvf_data(float f);
  void gvf_data(vector<float> vect);
  
  // Metadata
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
  
  BodyPart gvf_feature; // Feature studied
  vector<float> get_body_part(SkeletonDataPoint data_point, BodyPart part);
  
  // FEATURE EXTRACTION
  ofVec3f get_segment(ofPoint joint1, ofPoint joint2);
  float get_angle(ofVec3f segment1, ofVec3f segment2); // Angle between two arm segments. 
  
};

#endif /* defined(__mvf__gvfKinectHandler__) */
