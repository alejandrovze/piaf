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

#endif /* defined(__mvf__gvfKinectInterface__) */

class gvfKinectInterface {
  
public:
  
  void setup(gvfKinect* _kinect_app);
  void exit();
  
  void update();
  void draw();
  
  
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
//  
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
//  ofxButton save;
//  ofxButton load;
//  
//  void saveGestures();
//  void loadGestures();
//  

//  
//  // --- INPUTS GUI ---
//  ofxPanel inputsGui;
//  void initialiseInputsGui();
//  void updateInputsGui(vector<float> inputData);
//  
//  vector<gvfInputInfo> inputsInfo;
//  
//  int nAccelerometers;
//  vector<ofxVec3SliderDisplay> inputSliders; // FIXME: Generalize dimensions?
//  ofxVec3SliderDisplay accSlider1;
//  ofxVec3SliderDisplay accSlider2;
//  ofxVec3SliderDisplay kinectSlider;
//  
//  // --- GESTURE GUI ---
//  
//  ofxUICanvas* gesture_gui;
//  ofxUIMovingGraphX* current_gesture_graph;
//  void initialiseGestureGui(vector<vector<float> >* gesture_buffer);
//  void updateGestureGui();
//  
//  
//  // --- TEMPLATES GUI ---
//  
//  ofxUICanvas* templates_gui;
//  void initialiseTemplatesGui();
//  void updateTemplatesGui(int nTemplates);
//  vector<templateVisualiser> templates;
//  void updateTemplates(int nTemplates);
  
  
};