//
//  PianoInput.h
//  piaf
//
//  Created by Alejandro Van Zandt-Escobar on 09/07/2014.
//
//

#ifndef __piaf__PianoInput__
#define __piaf__PianoInput__

#include <iostream>

#include "ofxMidi.h"

class PianoInput : public ofxMidiListener {
	
public:
    
    void setup();
    
    // MARK: Midi Input
    ofxMidiIn midiIn;
	ofxMidiMessage midiMessage;
    
	void newMidiMessage(ofxMidiMessage& eventArgs);
    
private:
    
    // MARK: Audio setup
	void audioIn (float * input, int bufferSize, int nChannels); /* input method */
    void audioOut(float * output, int bufferSize, int nChannels); // output method
    
    double outputs[2];
    int nOutputChannels;
    int nInputChannels;
	int nBuffers;
	int	initialBufferSize; /* buffer size */
    int	sampleRate;
    
};



#endif /* defined(__piaf__PianoInput__) */
