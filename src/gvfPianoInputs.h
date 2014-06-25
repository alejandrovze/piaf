//
//  gvfPianoInputs.h
//  oFxGVFxPiano
//
//  Created by Alejandro Van Zandt-Escobar on 26/11/2013.
//
//

#ifndef __oFxGVFxPiano__gvfPianoInputs__
#define __oFxGVFxPiano__gvfPianoInputs__

#include <iostream>

#include "ofxOsc.h"
#include "ofMain.h"

#include "KinectInput.h"

// Struct is used so we can pass information on inputs from here, so that this
// information does not need to be held anywhere else.
// Implementation could be improved but works for now.
typedef struct {
    string inputName;
    int inputDimension;
    vector<vector<float> > inputBounds;
} gvfInputInfo;

class gvfPianoInputs {
    
public:
    
    void setup();
    void update();
    vector<float> initialise();
    
    vector<float> getInputData();
    vector<gvfInputInfo> getInputsInfo();
    void setInputs(bool _accOneOn, bool _accTwoOn, bool _leftHandOn, bool _rightHandOn);
    int getInputSize();
    // TODO: setInputBounds function
    
    void setKinectMode(); //TODO: Implement this to switch between OpenCV and Synapse approaches.
    
    KinectInput* get_kinect_input();
    
private:
    
    bool leftHandOn;
    bool rightHandOn;
    bool accOneOn;
    bool accTwoOn;
    
    vector<gvfInputInfo> inputsInfo;
    vector<float> inputData;
    int inputDataSize;
    vector<float> initial_point;
    void storeInput();
    
    // Kinect Input
    KinectInput kinect_input;
    SkeletonDataPoint current_point;
    ofPoint leftHand;
    ofPoint rightHand;
    ofPoint head;
    vector<vector<float> > kinectBounds;
    bool kinect_is_live;
    
    // Accelerometer Input
    ofxOscReceiver accReceiver;
    vector<string> accAddresses;
    vector<vector <float> > accData;
    vector<vector<float> > accelerometerBounds;
    
    vector<float> getAccData(ofxOscReceiver& accReceiver, string address, int accId);
    
    
    //    // MARK: Kinect with (blob tracking)
    //    // TODO: Finish implementing (so can use both alternatives).
    //    ofxKinect kinect;
    //    int angle;
    //    ofxCvColorImage colorImg;
    //	ofxCvGrayscaleImage grayImage; // grayscale depth image
    //	ofxCvGrayscaleImage grayThreshNear; // the near thresholded image
    //	ofxCvGrayscaleImage grayThreshFar; // the far thresholded image
    //    ofxCvContourFinder contourFinder;
    //
    //    int nearThreshold;
    //    int farThreshold;
    //
    //    void kinectSetup();
    //    void kinectUpdate();
    
    
};

#endif /* defined( __oFxGVFxPiano__gvfPianoInputs__) */


