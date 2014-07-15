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
    
    // Midi Setup
    keys_listener.setup(1);
    control_listener.setup(2);

}


ofEvent<ofxMidiMessage>& PianoInput::GetPianoEvent() {
    return keys_listener.midi_event;
}

ofEvent<ofxMidiMessage>& PianoInput::GetControlEvent() {
    return control_listener.midi_event;
    
}

void PianoInput::MidiChannelListener::setup(int port) {
    
    midiIn.openPort(port);
    midiIn.ignoreTypes(false, false, false);
    midiIn.addListener(this);
    midiIn.setVerbose(true);
    cout << midiIn.getName() << endl; // Check port
}

void PianoInput::MidiChannelListener::newMidiMessage(ofxMidiMessage& msg) {
    
    ofNotifyEvent(midi_event, msg, this);
    
}

                          
