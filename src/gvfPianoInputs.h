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

#include "ofMain.h"

#include "ofxCsv.h"
#include "ofxOsc.h"
#include "KinectInput.h"

class gvfPianoInputs {
    
public:
    
    void setup();
    bool update();
    vector<float> initialise();
    
    vector<float> getInputData();
    void setInputs(bool _accOneOn, bool _accTwoOn, bool _leftHandOn, bool _rightHandOn);
    int getInputSize();
    
    KinectInput* get_kinect_input();
    
    void StartFile();
    void EndFile();
    void LoadFile(string filename);
    
private:
    
    bool leftHandOn;
    bool rightHandOn;
    bool accOneOn;
    bool accTwoOn;
    
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
    bool kinect_is_live;
    
    // Accelerometer Input
    ofxOscReceiver accReceiver;
    vector<string> accAddresses;
    vector<vector <float> > accData;
    vector<vector<float> > accelerometerBounds;
    
    vector<float> getAccData(ofxOscReceiver& accReceiver, string address, int accId);
    
    // CSV Recorder
    bool is_writing;
    wng::ofxCsv csv_recorder;
    bool is_reading;
    wng::ofxCsv csv_reader;
    int csv_row;
    
    void WriteCsvData(wng::ofxCsv* csv_recorder);
    bool ReadCsvData();
};

#endif /* defined( __oFxGVFxPiano__gvfPianoInputs__) */


