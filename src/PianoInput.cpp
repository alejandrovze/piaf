//
//  PianoInput.cpp
//  piaf
//
//  Created by Alejandro Van Zandt-Escobar on 09/07/2014.
//
//

#include "PianoInput.h"

//--------------------------------------------------------------
void PianoInput::setup() {
    
    
    // MARK: Midi Input
    // ================
    midiIn.openPort(1);
    midiIn.ignoreTypes(false, false, false);
    midiIn.addListener(this);
    midiIn.setVerbose(true);
    cout << midiIn.getName(); // Check port
    
    // MARK: AUDIO SETUP
    // =================
    // 2 output channels,
    // 1 input channels
    // 44100 samples per second
    // 4 num buffers (latency)
    
    //	sampleRate 			= 44100; /* Sampling Rate */
    //	initialBufferSize	= 512;	/* Buffer Size. you have to fill this buffer with sound*/
    //  nOutputChannels = 2;
    //  nInputChannels = 1;
    //  nBuffers = 4;
    //
    //
    //	ofSoundStreamSetup(nOutputChannels, nInputChannels, this, sampleRate, initialBufferSize, nBuffers);
}




//--------------------------------------------------------------
void PianoInput::newMidiMessage(ofxMidiMessage& msg) {
    
	// make a copy of the latest message
	midiMessage = msg;
    
    // Press is start, release is stop when learning
    // Release (or press) triggers for following
    
    // Pedal Input Control Number: 64
    if (midiMessage.control == 64) {
        // Toggle Playing
    }
    
}
