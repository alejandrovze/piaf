#include "ofApp.h"

string testString;
//--------------------------------------------------------------
void ofApp::setup(){
    
	ofSetCircleResolution(50);
    
	ofSetWindowTitle("openframeworks gvf visualiser");
    ofSetWindowShape(1024, 768);
    
	ofSetFrameRate(60); // if vertical sync is off, we can go a bit fast... this caps the framerate at 60fps.
    
    ofPoint wSize = ofGetWindowSize();
    scrW = wSize.x;
    scrH = wSize.y;
    
    // SETUP ELEMENTS
    inputs.setup();                         // inputs devices
    handler.setup(inputs.getInputSize());   // gvf
    interface.setup(&handler, &inputs);  // interface
    
    sender.setup();
    
    initColors();
    
    // some standard setup stuff
	ofEnableAlphaBlending();
	ofSetupScreen();
	ofBackground(0, 0, 0);
	ofSetFrameRate(60);
    
    
    // MARK: Midi Input
    // ================
    //	midiIn.openPort(1);
    //	midiIn.ignoreTypes(false, false, false);
    //	midiIn.addListener(this);
    //	midiIn.setVerbose(true);
    //    cout << midiIn.getName(); // Check port
    
    
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
void ofApp::update(){
    
    inputs.update();
    
    // Feed data from inputs to gvf
    if (handler.getIsPlaying()) {
        handler.gvf_data(inputs.getInputData());
    }
    
    interface.update();
    
    recognitionInfo mostProb = handler.getRecogInfoOfMostProbable();
    sender.SendGVFOutcome(handler.getGVF()->getMostProbableGestureIndex(),
                          mostProb.probability, mostProb.phase, mostProb.speed,
                          mostProb.scale, mostProb.rotation);
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    float templatesScale = 1.0f; //Leave scale at 1.0.
    ofBackgroundGradient(ofColor(2), ofColor(40), OF_GRADIENT_CIRCULAR);
    ofPushMatrix();
    
    interface.draw();
    
    ofDisableAlphaBlending();
}

//--------------------------------------------------------------
void ofApp::exit() {
    
    // TODO: Shutdown for Inputs (Close Kinect, etc.)
    
    interface.exit();
}

//--------------------------------------------------------------
void ofApp::newMidiMessage(ofxMidiMessage& msg) {
    
	// make a copy of the latest message
	midiMessage = msg;
    
    // Press is start, release is stop when learning
    // Release (or press) triggers for following
    
    
    
    /*
     // Pedal Input Control Number: 64
     if (midiMessage.control == 64) {
     if (application.getState() == ofxGVF::STATE_LEARNING) {
     if (midiMessage.value == 0) {
     application.setFollowing(true);
     }
     else if (midiMessage.value == 127) {
     application.setFollowing(false);
     }
     }
     else {
     if (midiMessage.value == 127) {
     // Toggle Following
     application.setFollowing(!application.getFollowing());
     }
     }
     }
     */
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    // Set GVF to Learning State
	if (key == 'l' || key == 'L'){
        handler.setState(ofxGVF::STATE_LEARNING);
	}
    // Set GVF to Following State 
    else if (key == 'f' || key == 'F') {
        handler.setState(ofxGVF::STATE_FOLLOWING);
    }
    // Clear GVF
    else if(key == 'c' || key == 'C')
    {
        handler.setState(ofxGVF::STATE_CLEAR);
    }
    // Toggle Is Following
    else if (key == ' ') {
        handler.toggleIsPlaying();
    }
}


//MARK: GUI
//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    ofPoint wSize = ofGetWindowSize();
    scrW = wSize.x;
    scrH = wSize.y;
    printf("w: %d h: %d\n", scrW, scrH);
}


void ofApp::initColors()
{
    colors.clear();
    colors.push_back(ofColor::white);
    colors.push_back(ofColor::gray);
    colors.push_back(ofColor::blue);
    colors.push_back(ofColor::cyan);
    colors.push_back(ofColor::olive);
    colors.push_back(ofColor::gold);
    colors.push_back(ofColor::magenta);
    colors.push_back(ofColor::violet);
}

ofColor ofApp::generateRandomColor()
{
    ofColor c;
    
    if(colors.size() == 0)
        initColors();
    
    int colorsRemaining = colors.size();
    
    int index = ofRandom(0, colorsRemaining - 1);
    
    c = colors[index];
    colors.erase(colors.begin() + index);
    return c;
}



//---------------------

void ofApp::guiEvent(ofxUIEventArgs &e)
{
//	string name = e.widget->getName();
//	int kind    = e.widget->getKind();
//    
//	cout << "got event from: " << name << endl;
}