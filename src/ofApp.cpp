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
  printf("w: %d h: %d\n", scrW, scrH);
  
  gvf_kinect.setup();
  
  kinect_interface.setup(&gvf_kinect);
  
  initColors();
  
  /* some standard setup stuff*/
	ofEnableAlphaBlending();
	ofSetupScreen();
	ofBackground(0, 0, 0);
	ofSetFrameRate(60);
  
}


//--------------------------------------------------------------
void ofApp::update(){
  
  gvf_kinect.update();
  
  kinect_interface.update();
  
}

//--------------------------------------------------------------
void ofApp::draw(){
  
  ofBackgroundGradient(ofColor(2), ofColor(40), OF_GRADIENT_CIRCULAR);
  ofPushMatrix();
  
  ofDisableAlphaBlending();
  
  // show the current frame rate
  ofDrawBitmapString("FPS " + ofToString(ofGetFrameRate(), 0), ofGetWidth() - 200, 25);
  
  // Display state
  string state;
  switch (gvf_kinect.get_state()) {
    case ofxGVF::STATE_CLEAR:
      state = "clear";
      break;
    case ofxGVF::STATE_LEARNING:
      state = "learn";
      break;
    case ofxGVF::STATE_FOLLOWING:
      state = "follow";
      break;
  }
  string is_playing;
  
  if (gvf_kinect.get_is_playing()) {
    is_playing = "Is Playing";
  }
  else {
    is_playing = "Is Not Playing";
  }
  int n_templates = gvf_kinect.get_n_templates();
  
  ofDrawBitmapString(state + " " + is_playing + " " + ofToString(n_templates) + "templates", ofGetWidth() - 500, 60);
  
  gvf_kinect.draw();
  
  kinect_interface.draw();
  
}

//--------------------------------------------------------------
void ofApp::exit() {
  
  kinect_interface.exit();
  
  gvf_kinect.exit();
  
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
  
	if (key == 'l' || key == 'L') {
    gvf_kinect.learn();
	}
  else if (key == 'f' || key == 'F') {
    gvf_kinect.follow();
  }
  else if(key == 'c' || key == 'C') {
    gvf_kinect.clear();
  }
  else if (key == ' ') {
    // Toggle Playing
    if (gvf_kinect.get_is_playing())
      gvf_kinect.stop();
    else
      gvf_kinect.play();
  }
  else if (key == 's' || key == 'S') {
    gvf_kinect.saveGestures("new_file.xml");
  }
  else if (key == 'x' || key == 'X') {
    gvf_kinect.loadGestures();
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
  //cout << index << endl;
  c = colors[index];
  colors.erase(colors.begin() + index);
  return c;
}



////---------------------
//
//void ofApp::guiEvent(ofxUIEventArgs &e)
//{
//	string name = e.widget->getName();
//	int kind = e.widget->getKind();
//  
//	cout << "got event from: " << name << endl;
//	
//}
//
////!!!: Event handling not implemented with restructuring.
//void ofApp::templatesGuiEvent(ofxUIEventArgs &e) {
//  //    gvfh.templateGuiEvent(e);
//}
