//
//  gvfKinectInterface.h
//  mvf
//
//  Created by Alejandro Van Zandt-Escobar on 02/04/2014.
//
//

#ifndef __mvf__gvfKinectInterface__
#define __mvf__gvfKinectInterface__

#include <iostream>

#include "ofxGui.h"
#include "ofxUI.h"

#include "gvfKinect.h"

class gvfKinectInterface {
  
public:
  
  void setup(gvfKinect* _kinect_app);
  void exit();
  
  void update();
  void draw();
  
  
  // TEMPLATES
  int get_template_id();
  void set_template_id(int _template_id);
  
  
  
private:
  
  gvfKinect* kinect_app;
  
  
  // --- STATUS GUI ---
  ofxUICanvas* status_gui;
  void initialiseStatusGui();
  void updateStatusGui();
  
//  ofParameter<string> currentState;
//  ofParameter<bool> inputOnOff;
//
//  ofParameterGroup mostProbableStatus;
//  ofParameter<int> mostProbableIndex;
//  ofParameter<float> mostProbablePhase;
//  ofParameter<float> mostProbableSpeed;
//  ofParameter<float> mostProbableScale;

//  void drawString(); // ??? Instructions etc. Do we want this?
  
  // -------------------
  
  // --- KINECT GUI ---
  void InitialiseKinectGui();
  void UpdateKinectGui();
  
  ofImage depth_image;
  unsigned char *grayPixels;
  
  // Dimensions of frame in which to display Kinect.
  int kinect_width;
  int kinect_height;
  // Dimensions of original depth image. 
  int depth_width;
  int depth_height;
  
  void UpdateDepth(openni::VideoFrameRef depth_frame);
  float* CalculateHistogram(int histogramSize, const openni::VideoFrameRef& depthFrame);
  
  void UpdateSkeleton(SkeletonDataPoint new_point);
  void DisplaySkeleton(SkeletonDataPoint new_point, ofPoint location);
  // -------------------
  
  // --- TEMPLATES GUI ---
  void InitialiseTemplatesGui();
  void UpdateTemplatesGui();
  
  ofPoint template_location;
  int template_id;
  int template_position; // Playback position
  SkeletonGesture* current_template;
  // -------------------
  
  

  //  // --- TEMPLATES GUI ---
  //
  //  ofxUICanvas* templates_gui;
  //  void initialiseTemplatesGui();
  //  void updateTemplatesGui(int nTemplates);
  //  vector<templateVisualiser> templates;
  //  void updateTemplates(int nTemplates);
  
//  // --- SETTINGS GUI ---
//  ofxPanel settingsGui;
//  void initialiseSettingsGui();
//  
//  ofParameterGroup settings;
//  ofParameter<int> numParticles;
//  ofParameter<int> resampleThreshold;
//  ofParameter<float> smoothingCoefficient;
//  
//  ofParameterGroup varianceCoefficients;
//  ofParameter<float> sigPosition;
//  ofParameter<float> sigRotation;
//  ofParameter<float> sigScale;
//  ofParameter<float> sigSpeed;
//  
//  void numParticlesChanged(int & numParticles);
//  void resampleThresholdChanged(int & resampleThreshold);
//  void smoothingCoefficientChanged(float & smoothingCoefficient);
//  void varianceCoefficentsChanged(float & coefficent);
//  


//  
//  // --- GESTURE GUI ---
//  
//  ofxUICanvas* gesture_gui;
//  ofxUIMovingGraphX* current_gesture_graph;
//  void initialiseGestureGui(vector<vector<float> >* gesture_buffer);
//  void updateGestureGui();
//  
//  

  
  
};


#endif /* defined(__mvf__gvfKinectInterface__) */