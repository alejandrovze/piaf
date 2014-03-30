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

// struct to hold information on how a learnt template is being recognised.
//typedef struct {
//    float likelihoods;
//    float probability;
//    float phase;
//    float speed;
//    float scale;
//    float rotation;
//}  recognitionInfo;

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
  
  // vector containing one recognitionInfo struct for each template
  // these structs will contain real time information on how each template is being recognised
  //    std::vector<recognitionInfo> recogInfo;
  //    std::vector<recognitionInfo> getRecogInfo();
  //    recognitionInfo getRecogInfoOfMostProbable();

  // amount of templates
  int getTemplateCount();
  
  // returns the index of the template that is, most probably,
  // the one the user is trying to perform
  int getIndexMostProbable();
  
private:
  
  ofxGVF *mygvf;
  
};

#endif /* defined(__mvf__gvfKinectHandler__) */
