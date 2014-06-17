//
//  gvfPianoInputs.cpp
//  oFxGVFxPiano
//
//  Created by Alejandro Van Zandt-Escobar on 26/11/2013.
//
//

#include "gvfPianoInputs.h"

//--------------------------------------------------------------
void gvfPianoInputs::setup(){

  
  kinect_input.setup();
  
  if (kinect_input.get_is_running())
    kinect_is_live = true; // Only set to live if correctly set up the kinect.
  
    leftHand = vector<double>(3,0.0);
    rightHand = vector<double>(3,0.0);
    
    // TODO: Write method for bounds setting.
    vector<float> kBounds;
    kBounds.push_back(-500.);
    kBounds.push_back(500.);
    kinectBounds = vector<vector<float> >(3, kBounds);
    
    // MARK: Accelerometer
//    accAddresses.push_back("/wax/120");
//    accAddresses.push_back("/wax/121");
    accAddresses.push_back("/wax/14");

    int port = 8200;
    accReceiver.setup(port);
    
    // Initialize data input vector
    accData = std::vector<vector<float> >(2, vector<float>(3,0.0));
    
    // TODO: Write method for bounds settings. 
    vector<float> aBounds;
    aBounds.push_back(-1.);
    aBounds.push_back(1.);
    accelerometerBounds = vector<vector<float> >(3, aBounds);
    
    // Default is track right hand, one accelerometer.
    setInputs(true, false, false, false);

}

//--------------------------------------------------------------
void gvfPianoInputs::update(){
  
  if (kinect_is_live) {
    kinect_input.update();
    current_point = kinect_input.get_data();
    
    
    leftHand[0] = current_point.joints[nite::JOINT_LEFT_HAND].x;
    leftHand[1] = current_point.joints[nite::JOINT_LEFT_HAND].y;
    leftHand[2] = current_point.joints[nite::JOINT_LEFT_HAND].z;
    
    rightHand[0] = current_point.joints[nite::JOINT_RIGHT_HAND].x;
    rightHand[1] = current_point.joints[nite::JOINT_RIGHT_HAND].y;
    rightHand[2] = current_point.joints[nite::JOINT_RIGHT_HAND].z;
  }
  
  
//    synapseStreamer.parseIncomingMessages();
//    
//    if( synapseStreamer.getNewMessage() ){
//        
//        leftHand = synapseStreamer.getLeftHandJointBody();
//        //!!! Quick & dirty scaling
//        leftHand.at(0) = leftHand.at(0) / 500.; // range -500 to 500
//        leftHand.at(1) = leftHand.at(1) / 1000.;// range 0 to 1000
//        leftHand.at(2) = leftHand.at(2) / 500.; // range -500 to 500
//        rightHand = synapseStreamer.getRightHandJointBody();
//        // !!! Quick & dirty scaling
//        rightHand.at(0) = rightHand.at(0) / 500.; // range -500 to 500
//        rightHand.at(1) = rightHand.at(1) / 1000.;// range 0 to 1000
//        rightHand.at(2) = rightHand.at(2) / 500.; // range -500 to 500
//    }
  
    
  
    
    // Get data from 3D accelerometer.
    while (accReceiver.hasWaitingMessages()) {
        accData.at(0) = getAccData(accReceiver, "/wax/14", 0);
//        accData.at(0) = getAccData(accReceiver, "/wax/120", 0);
//        accData.at(1) = getAccData(accReceiver, "/wax/121", 1);
    }
    
    storeInput();
}

void gvfPianoInputs::storeInput() {
    // Store in input vector.
    int inputIterator = 0;
    if (accOneOn) {
        for (int i = 0; i < 3; ++i) {
            inputData.at(i + inputIterator) = (accData.at(0)).at(i);
        }
        inputIterator += 3;
    }
    if (accTwoOn) {
        for (int i = 0; i < 3; ++i) {
            inputData.at(i + inputIterator) = (accData.at(1)).at(i);
        }
        inputIterator += 3;
    }
    if (leftHandOn) {
        for (int i = 0; i < 3; ++i) {
            inputData.at(i + inputIterator) = leftHand.at(i);
        }
        inputIterator += 3;
    }
    if (rightHandOn) {
        for (int i = 0; i < 3; ++i) {
            inputData.at(i + inputIterator) = rightHand.at(i);
        }
        inputIterator += 3;
    }
}

// Returns initial point.
//--------------------------------------------------------------
std::vector<float> gvfPianoInputs::initialise() {
    initial_point = inputData;
    storeInput(); // Update with initial point.
    return initial_point;
}

KinectInput* gvfPianoInputs::get_kinect_input() {
  return &kinect_input;
}

//--------------------------------------------------------------
std::vector<float> gvfPianoInputs::getInputData() {
    return inputData;
}

//--------------------------------------------------------------
std::vector<gvfInputInfo> gvfPianoInputs::getInputsInfo() {
    return inputsInfo;
}

int gvfPianoInputs::getInputSize() {
    return inputDataSize;
}

//--------------------------------------------------------------
void gvfPianoInputs::setInputs(bool _accOneOn, bool _accTwoOn,
                               bool _leftHandOn, bool _rightHandOn) {
    accOneOn = _accOneOn;
    accTwoOn = _accTwoOn;
    leftHandOn = _leftHandOn;
    rightHandOn = _rightHandOn;
    
    if (accOneOn) {
        gvfInputInfo accOne = {"Accelerometer 1", 3, accelerometerBounds};
        inputsInfo.push_back(accOne);
    }
    if (accTwoOn) {
        gvfInputInfo accTwo = {"Accelerometer 2", 3, accelerometerBounds};
        inputsInfo.push_back(accTwo);
    }
    if (leftHandOn) {
        gvfInputInfo kinectLeft = {"Kinect Left Hand", 3, kinectBounds};
        inputsInfo.push_back(kinectLeft);
    }
    if (rightHandOn) {
        gvfInputInfo kinectRight = {"Kinect Right Hand", 3, kinectBounds};
        inputsInfo.push_back(kinectRight);
    }
    
    // TODO: Replace by iterating through inputsInfo vector
    inputDataSize = 3 * (leftHandOn + rightHandOn + accOneOn + accTwoOn);
    inputData = std::vector<float>(inputDataSize, 0.0);
    initial_point = std::vector<float>(inputDataSize, 0.0);
    
}

//MARK: Data Input
//--------------------------------------------------------------
//Data Input: Accelerometer Data
std::vector<float> gvfPianoInputs::getAccData(ofxOscReceiver& accReceiver, string address, int accId) {
    
    ofxOscMessage accMessage;
    
    std::vector<float> accVector = vector<float>(3,0.0);
    
    accReceiver.getNextMessage(&accMessage);
    
    if (accMessage.getAddress() == address) {
        for (int i = 0; i < 3; i++)
            accVector.at(i) = accMessage.getArgAsFloat(i);
    }
    else {
        return accData.at(accId);
    }
    
    return accVector;
    
}