#include "ofApp.h"

string testString;
//--------------------------------------------------------------
void ofApp::setup(){
    
	ofSetCircleResolution(50);
    
	ofSetWindowTitle("openframeworks gvf visualiser");
    ofSetWindowShape(1024, 768);
    
	ofSetFrameRate(60); // if vertical sync is off, we can go a bit fast... this caps the framerate at 60fps.
    
    // SETUP ELEMENTS
    inputs.setup();                         // inputs devices
    handler.init(inputs.get_input_size());   // gvf
    interface.setup(&handler, &inputs);  // interface
    
    ofAddListener(inputs.GetPianoEvent(), this, &ofApp::midiNotes);
    ofAddListener(inputs.GetControlEvent(), this, &ofApp::midiControl);
    
    sender.setup();
    
    // some standard setup stuff
	ofEnableAlphaBlending();
	ofSetupScreen();
	ofBackground(0, 0, 0);
	ofSetFrameRate(60);
    
}


//--------------------------------------------------------------
void ofApp::update(){
    
    if(!inputs.update() && handler.getIsPlaying()) {
        handler.toggleIsPlaying();
    }
    
    // Feed data from inputs to gvf
    if (handler.getIsPlaying()) {
        handler.gvf_data(inputs.ConcatenateInput());

    }
    
    interface.update();
    
    Estimation mostProb = handler.getRecogInfoOfMostProbable();
    sender.SendGVFOutcome(handler.getMostProbableGestureIndex(),
                          mostProb.probability, mostProb.phase, mostProb.speed,
                          mostProb.scale, mostProb.rotation);
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
//    ofBackgroundGradient(ofColor::black, ofColor(40), OF_GRADIENT_CIRCULAR);
    
    ofMatrix4x4 matrix = ofGetCurrentMatrix(OF_MATRIX_MODELVIEW);
    
    ofPushMatrix();
    
    interface.draw();
    
    ofPopMatrix();
    
    ofDisableAlphaBlending();
}

//--------------------------------------------------------------
void ofApp::exit() {
    
    // TODO: Shutdown for Inputs (Close Kinect, etc.)
    
    interface.exit();
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    // Set GVF to Learning State
	if (key == 'l' || key == 'L'){
        handler.set_state(ofxGVF::STATE_LEARNING);
	}
    // Set GVF to Following State 
    else if (key == 'f' || key == 'F') {
        handler.set_state(ofxGVF::STATE_FOLLOWING);
    }
    // Clear GVF
    else if(key == 'c' || key == 'C')
    {
        handler.set_state(ofxGVF::STATE_CLEAR);
    }
    // Toggle Is Following
    else if (key == ' ') {
        handler.toggleIsPlaying();
        // FIXME: Must be a cleaner / more elegant way for this.
        if (handler.getIsPlaying()) {
            inputs.StartFile();
        }
        else {
            inputs.EndFile();
        }
    }
    else if (key == 'i' || 'I') {
        if (!handler.getIsPlaying()) {
            LoadInputFile();
            handler.toggleIsPlaying();
        }
    }
}

//--------------------------------------------------------------
void ofApp::midiNotes(ofxMidiMessage& msg){
    
    if (msg.control == 64) {
        if(msg.value == 0) {
            handler.toggleIsPlaying();
        }
    }
    
}

//--------------------------------------------------------------
void ofApp::midiControl(ofxMidiMessage& msg) {
    // Change States
    if (msg.control == 82) {
        if (msg.value == 0) {
            switch (msg.channel) {
                case 1:
                    handler.set_state(ofxGVF::STATE_LEARNING);
                    break;
                case 2:
                    handler.set_state(ofxGVF::STATE_FOLLOWING);
                    break;
                case 3:
                    handler.set_state(ofxGVF::STATE_CLEAR);
                    break;
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::LoadInputFile() {

    string filename;
    
    ofFileDialogResult dialogResult = ofSystemLoadDialog("Select the file containing gesture to be played.");
    if(!dialogResult.bSuccess)
        return;
    
    stringstream ss;
    ss << dialogResult.filePath;
    filename = ss.str();
    cout << filename;
    
    inputs.LoadFile(filename);
    cout << "Playback gesture loaded.\n";

}



