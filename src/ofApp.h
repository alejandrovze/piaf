#pragma once

#include "ofMain.h"
#include "gvfKinect.h"

class ofApp : public ofBaseApp {
	
public:
  
  void setup();
  void update();
  void draw();
  
  void exit();
  
  void keyPressed(int key);
  void windowResized(int w, int h);
//  
//  void guiEvent(ofxUIEventArgs &e);
//  void templatesGuiEvent(ofxUIEventArgs &e);
  
  gvfKinect gvf_kinect;
  
private:
  
//  gvfPianoInterface ofGui;
  
  int scrW, scrH;
  
  void initColors();
  ofColor generateRandomColor();
  std::vector<ofColor> colors;
  
};


