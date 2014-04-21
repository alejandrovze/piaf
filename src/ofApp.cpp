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
  
  //------------------------------------
  
  gvf_kinect.setup();
  
  kinect_interface.setup(&gvf_kinect);
  
  osc_interface.setup();
  
  //------------------------------------
  
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
  
  osc_interface.update();
  
}

//--------------------------------------------------------------
void ofApp::draw(){
  
  ofBackgroundGradient(ofColor(2), ofColor(40), OF_GRADIENT_CIRCULAR);
  ofPushMatrix();
  
  ofDisableAlphaBlending();
  
  ofSetColor(150, 150, 150);
  
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
  int n_records = gvf_kinect.get_n_records();
  int current_record = gvf_kinect.get_current_record();
  int playback_position = gvf_kinect.get_playback_position();
  int current_record_length = gvf_kinect.get_current_record_length();
  string mode;
  
  if (gvf_kinect.get_is_live()) {
    mode = "live";
  }
  else {
    mode = "vcr";
  }
  
  ofDrawBitmapString(state + " " + is_playing + " " + ofToString(n_templates) + "templates", ofGetWidth() - 400, 60);
  ofDrawBitmapString(state + " " + is_playing + " " + ofToString(n_records) + " records", ofGetWidth() - 400, 80);
  ofDrawBitmapString(ofToString(current_record) + " is current, mode is " + mode, ofGetWidth() - 400, 100);
  ofDrawBitmapString(ofToString(playback_position) + " of " + ofToString(current_record_length), ofGetWidth() - 400, 120);
  
  for (int i = 0; i < gvf_kinect.get_n_gvfs(); ++i) {
    
    ofDrawBitmapString(gvf_kinect.get_gvf_name(i) + " returns most probable " +
                       ofToString(gvf_kinect.get_most_probable(i)), ofGetWidth() - 400, 140 + 20 * i);
    
  }
  
  //------------------------------------
  
  kinect_interface.draw();
  
  //------------------------------------
  
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
  else if (key == 'v') {
    // Toggle "live" mode (kinect vs recorded gestures).
    gvf_kinect.set_live(!gvf_kinect.get_is_live());
  }
  else if (key == 's' || key == 'S') {
    if (!gvf_kinect.get_is_playing())
      gvf_kinect.saveGestures("new_file.xml");
    else
      cout << "Can only load when stopped." << endl;
  }
  else if (key == 'x' || key == 'X') {
    if (!gvf_kinect.get_is_playing())
      gvf_kinect.loadGestures();
    else
      cout << "Can only load when stopped." << endl;
  }
  // Switch current gesture
  else if (key == ',') {
    gvf_kinect.set_current_record(gvf_kinect.get_current_record() - 1);
  }
  else if (key == '.') {
    gvf_kinect.set_current_record(gvf_kinect.get_current_record() + 1);
  }
  // Switch template gesture (display only)
  else if (key == ';') {
    kinect_interface.set_template_id(kinect_interface.get_template_id() - 1);
  }
  else if (key == '\'') {
    kinect_interface.set_template_id(kinect_interface.get_template_id() + 1);
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
