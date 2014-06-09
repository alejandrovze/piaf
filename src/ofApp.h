//
//  Adapted by Alejandro Van Zandt-Escobar
// Updated for openFrameworks 0.8


#pragma once

#include "ofMain.h"
#include "gvfPianoInterface.h"

#include "ofxUI.h"
#include "ofxGui.h"
#include "ofxMidi.h"

#include "gvfPianoApp.h"

class ofApp : public ofBaseApp, public ofxMidiListener {
	
public:
  
  void setup();
  void update();
  void draw();
  
  void exit();
  
  void keyPressed(int key);
  void windowResized(int w, int h);
  
  void guiEvent(ofxUIEventArgs &e);
  void templatesGuiEvent(ofxUIEventArgs &e);
  
  
  
  // MARK: Midi Input
  ofxMidiIn midiIn;
	ofxMidiMessage midiMessage;
  
	void newMidiMessage(ofxMidiMessage& eventArgs);
  
private:
  
  gvfPianoApp         application;
  gvfPianoInterface   interface;
  gvfPianoHandler     handler;
  gvfPianoInputs      inputs;
  
  bool isFollowing;
  
  int scrW, scrH;
  
  void initColors();
  ofColor generateRandomColor();
  std::vector<ofColor> colors;
  
//  // MARK: Audio setup
//	void audioIn (float * input, int bufferSize, int nChannels); /* input method */
//  void audioOut(float * output, int bufferSize, int nChannels); // output method
//  
//  double outputs[2];
//  int nOutputChannels;
//  int nInputChannels;
//	int nBuffers;
//	int	initialBufferSize; /* buffer size */
//	int	sampleRate;
  
};

