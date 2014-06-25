//
//  Adapted by Alejandro Van Zandt-Escobar
// Updated for openFrameworks 0.8


#pragma once

#include "ofMain.h"

#include "ofxMidi.h"

#include "piafInterface.h"
#include "piafOSCSender.h"

class ofApp : public ofBaseApp, public ofxMidiListener {
	
public:
    
    void setup();
    void update();
    void draw();
    
    void exit();
    
    void keyPressed(int key);
    void windowResized(int w, int h);
    
    void guiEvent(ofxUIEventArgs &e);
    
    // MARK: Midi Input
    ofxMidiIn midiIn;
	ofxMidiMessage midiMessage;
    
	void newMidiMessage(ofxMidiMessage& eventArgs);
    
private:
    
    piafInterface   interface;
    gvfPianoHandler     handler;
    gvfPianoInputs      inputs;
    
    piafOSCSender sender;
    
    
    // MARK: GUI
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

