//
//  gvfPianoApp.cpp
//  oFxGVFxPiano
//
//  Created by Alejandro Van Zandt-Escobar on 26/11/2013.
//
//

#include "gvfPianoApp.h"


gvfPianoApp::gvfPianoApp() {
    
    isFollowing = false;
    
    inputHandler.setup();
    
    cout << "input size: " << inputHandler.getInputSize() << "\n";
    
    gvfh = new gvfPianoHandler(inputHandler.getInputSize());
    gvf = gvfh->getGVF();
    

    gesture_data = vector<vector<float> >();

    
    templates_learned = 0;
}

//--------------------------------------------------------------
void gvfPianoApp::setup(){

//    currentGesture = new gvfGesture(PLAYING_GESTURE);
//    
//    isFollowing = false;
//
//    inputHandler.setup();
//    
//    cout << "input size: " << inputHandler.getInputSize() << "\n";
//    gvfh = gvfhandler(inputHandler.getInputSize());
//    
//    gesture_data = vector<vector<float> >();
//    
//    templates_learned = 0;

}

//--------------------------------------------------------------
void gvfPianoApp::update(){

    inputHandler.update();

    inputData = inputHandler.getInputData();

    if (isFollowing) {
        gestureInput(inputData);
        gesture_data.push_back(inputData);
    }

    
//    cout << "Size " << gesture_data.size() << "\n";
}

//--------------------------------------------------------------
void gvfPianoApp::gestureInput(std::vector<float> inputData){
    
    // If gesture has started, add new point to it.
    
//    currentGesture->addPoint(inputData); //!!!: Gesture to just the data. 
//    current_gesture.data.push_back(inputData);
    
    gvfh->gvf_data(inputData);
    
}

//--------------------------------------------------------------
void gvfPianoApp::gestureStart() {
    
//    gvfh->gvf_restart();
    
    gesture_data.clear();
    
//    currentGesture->initialise(inputData); //!!!: Eliminate
    
//    current_gesture.initial_point = inputHandler.initialise();
    inputData = inputHandler.getInputData(); // Update input (renormalized).
    
//    current_gesture.data.clear();
//    current_gesture.data.push_back(inputData);
    
    
    /*
    if(getState() == ofxGVF::STATE_LEARNING)
    {
        gvf->addTemplate(inputData);
    }
    else if(getState() == ofxGVF::STATE_CLEAR) {
        setState(ofxGVF::STATE_FOLLOWING);
    }*/
    
    isFollowing = true;
    
}

//--------------------------------------------------------------
void gvfPianoApp::gestureStop() {
    
    if(isFollowing)
    {
        isFollowing = false; // Stop gesture data input
    }
    else {
        cout << "Toggling Error\n"; //!!!: Temp for debugging
    }
    
    // !!!: Need to figure out better way to learn multiple templates.
    if(getState() == ofxGVF::STATE_LEARNING) {
        setState(ofxGVF::STATE_FOLLOWING);
        templates_learned = getTemplateCount();
//        setState(STATE_LEARNING);
    }
    
}


//MARK: Sets
//--------------------------------------------------------------
void gvfPianoApp::setState(ofxGVF::ofxGVFState state) {
    
    int currentState = getState();
    
    switch (state)
    {
        case ofxGVF::STATE_LEARNING:
            if (currentState != ofxGVF::STATE_LEARNING && !isFollowing) {
                gvfh->gvf_learn();
            }   
            break;
        case ofxGVF::STATE_FOLLOWING:
            if (currentState != ofxGVF::STATE_FOLLOWING && !isFollowing) {
                gvf->setState(ofxGVF::STATE_FOLLOWING);
//                gvfh->gvf_follow();
            }
            break;
        case ofxGVF::STATE_CLEAR:
            if (!isFollowing) { // Don't clear if playing.
                gvf->clear();
                gvfh->gvf_clear(); //!!!: Still need to do this because of "lastreferencelearned" business.
                templates_learned = getTemplateCount();
            }
            break;
    }
}

//--------------------------------------------------------------
void gvfPianoApp::setFollowing(bool _isFollowing) {
    
    if (isFollowing != _isFollowing) {
        if (!_isFollowing) { // Set to Stop
            gestureStop();
        }
        else { // Set to Start
            gestureStart();
        }
    }
    else {
        cout << "Toggling error\n"; // TODO: Temp, for debugging. 
    }
}

//--------------------------------------------------------------
void gvfPianoApp::setSmoothingCoefficient(float smoothingCoefficient)
{
    gvf->setTolerance(smoothingCoefficient);
}

//--------------------------------------------------------------
void gvfPianoApp::setResamplingThreshold(int resamplingThreshold)
{
    gvf->setResamplingThreshold(resamplingThreshold);
}

//--------------------------------------------------------------
/*void gvfPianoApp::setAdaptSpeed(ofxGVF::ofxGVFVarianceCoefficents varianceCoefficients)
{
    gvf->setVarianceCoefficents(varianceCoefficients);
}*/

//--------------------------------------------------------------
void gvfPianoApp::setNumberOfParticles(int numParticles) {
    gvf->setNumberOfParticles(numParticles);
}


//MARK: Gets
//--------------------------------------------------------------
ofxGVF::ofxGVFState gvfPianoApp::getState() {
    return gvf->getState();
}

//--------------------------------------------------------------
bool gvfPianoApp::getFollowing() {
    return isFollowing;
}

//--------------------------------------------------------------
vector<float> gvfPianoApp::getInputData() {
    return inputData;
}


vector<vector<float> >* gvfPianoApp::getGestureData() {
    return &gesture_data;
}

vector<gvfInputInfo> gvfPianoApp::getInputsInfo() {
    return inputHandler.getInputsInfo();
}

int gvfPianoApp::getIndexMostProbable() {
    return gvf->getMostProbableGestureIndex();
}

recognitionInfo gvfPianoApp::getInfoOfMostProbable() {
    return gvfh->getRecogInfoOfMostProbable();
}

int gvfPianoApp::getTemplatesLearned() {
//    cout << "GVF ntemplates is " << getTemplateCount() << "\n";
    return templates_learned;
}

int gvfPianoApp::getTemplateCount() {
    return gvf->getNumberOfGestureTemplates();
}

ofxGVFGesture gvfPianoApp::getTemplateData(int template_id) {
    return gvf->getGestureTemplate(template_id);
}

// Gets recognition info
recognitionInfo gvfPianoApp::getTemplateInfo(int template_id) {
    return gvfh->getTemplateRecogInfo(template_id);
}

// TODO
templateGesture gvfPianoApp::getTemplateGesture(int template_id) {
    templateGesture template_gesture;
    return template_gesture;
}


//vector<vector<float > >* gvfPianoApp::getCurrentGestureData() {
//    cout << "Current Gesture size " << current_gesture.data.size() << "\n";
//    return &(current_gesture.data);
//}


// MARK: Load / Save
//--------------------------------------------------------------
void gvfPianoApp::saveGestures(string filename) {
//    gvfh->saveGestures(filename);
}

//--------------------------------------------------------------
void gvfPianoApp::loadGestures(string filename) {
//    gvfh->loadGestures(filename);
//    gvfh->gvf_follow();
}


