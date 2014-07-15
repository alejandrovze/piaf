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

#include "ofMain.h"
#include "ofxMidi.h"

class PianoInput {
	
public:
    
    void setup();
    ofEvent<ofxMidiMessage>& GetPianoEvent();
    ofEvent<ofxMidiMessage>& GetControlEvent();
    
private:
    
    class MidiChannelListener : public ofxMidiListener {
        
    public:
        
        void setup(int port);
        ofEvent<ofxMidiMessage> midi_event;
        
    private:
        
        // MARK: Midi Input
        ofxMidiIn midiIn;
        ofxMidiMessage midiMessage;
        
        void newMidiMessage(ofxMidiMessage& eventArgs);
        
    };

private:    
    
    MidiChannelListener keys_listener;
    MidiChannelListener control_listener;
    
};



#endif /* defined(__piaf__PianoInput__) */
