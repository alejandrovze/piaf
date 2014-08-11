#include "ofApp.h"

string testString;
//--------------------------------------------------------------
void ofApp::setup(){
    
	ofSetCircleResolution(50);
    
	ofSetWindowTitle("openframeworks gvf visualiser");
    ofSetWindowShape(1024, 768);
    
    // Standard setup
	ofEnableAlphaBlending();
	ofSetupScreen();
	ofBackground(0, 0, 0);
	ofSetFrameRate(30);
    
    // SETUP ELEMENTS
    inputs.setup();
    handler.init(inputs.get_input_size(),
                 inputs.get_input_ranges()[0],
                 inputs.get_input_ranges()[1]);
    interface.setup(&handler, &inputs);
    
    ofAddListener(inputs.GetPianoEvent(), this, &ofApp::midiNotes);
    ofAddListener(inputs.GetControlEvent(), this, &ofApp::midiControl);
    
    sender.setup();
    
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
    
    ofxGVFEstimation mostProb = handler.getRecogInfoOfMostProbable();
    
    if (handler.getIsPlaying() && handler.getState() == ofxGVF::STATE_FOLLOWING) {
        
        ofxGVFOutcomes outcomes = handler.getOutcomes();
        int n_templates = outcomes.estimations.size();
        
        sender.SendInfo(n_templates, "/n_templates");
        sender.SendInfo(outcomes.most_probable, "/most_probable_id");
        
        for (int i = 0; i < n_templates; ++i) {
            sender.SendGVFOutcome(i, outcomes.estimations[i]);
        }
    }
    else { 
        sender.SendInfo(-1, "/most_probable_id");
    }
    
    for (int i = 0; i < inputs.get_mocap_data().size(); ++i) {
        sender.SendDataPoint(i, inputs.get_mocap_data()[i]);
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofPushMatrix();
    
    interface.draw();
    
    ofPopMatrix();
    
}

//--------------------------------------------------------------
void ofApp::exit() {
    
    // FIXME: Shutdown Kinect?
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
    else if (key == 'i' || key == 'I') {
        LoadInputFile();
    }
    else if (key == 'p' || key == 'P') {
        if (!handler.getIsPlaying()) {
            inputs.LoadFile(loaded_file);
            handler.toggleIsPlaying();
        }
    }
}

//--------------------------------------------------------------
void ofApp::midiNotes(ofxMidiMessage& msg){
    
    if (msg.control == 64) {
        if(msg.value == 0) {
            handler.toggleIsPlaying();
            if (handler.getIsPlaying()) {
                inputs.StartFile();
            }
            else {
                inputs.EndFile();
            }
        }
    }
    
    if (msg.control == 11) {
        if(msg.value == 0) {
            handler.toggleIsPlaying();
            if (handler.getIsPlaying()) {
                inputs.StartFile();
            }
            else {
                inputs.EndFile();
            }
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
    
    ofFileDialogResult dialogResult = ofSystemLoadDialog("Select the file containing gesture to be played.");
    if(!dialogResult.bSuccess)
        return;
    
    stringstream ss;
    ss << dialogResult.filePath;
    loaded_file = ss.str();
    cout << loaded_file;
    

}



