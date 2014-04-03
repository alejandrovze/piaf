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
  
  gvfKinectHandler();
  ~gvfKinectHandler();
  
  void setState(ofxGVF::ofxGVFState state);
  void addTemplate();
  void gvf_data(ofPoint p); // Kinect Input is 3D point.
  
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
  
};

#endif /* defined(__mvf__gvfKinectHandler__) */
