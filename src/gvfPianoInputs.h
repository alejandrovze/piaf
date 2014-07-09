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
#include "KinectInput.h"

class gvfPianoInputs {
    
public:
    
    void setup();
    bool update();
    vector<float> initialise();
    
    vector<float> getInputData();
    vector<bool> GetKinectJoints();
    vector<ofPoint> GetKinectData();
    void SetKinectJoints(vector<int> joints_on);
    void SetAccInputs(bool accOn, int acc);
    vector<bool> GetAccInputs();
    int getInputSize();
    
    KinectInput* get_kinect_input();
    
    void StartFile();
    void EndFile();
    void LoadFile(string filename);
    
private:
    
    vector<bool> kinect_joints_on;
    int n_joints;
    
    bool accOneOn;
    bool accTwoOn;
    
    vector<float> inputData;
    int inputDataSize;
    void storeInput();
    
    // Kinect Input
    KinectInput kinect_input;
    vector<ofPoint> current_point;
    bool kinect_is_live;
    
    // Accelerometer Input
    ofxOscReceiver accReceiver;
    vector<string> accAddresses;
    vector<vector <float> > accData;
    vector<vector<float> > accelerometerBounds;
    
    vector<float> getAccData(ofxOscReceiver& accReceiver, string address, int accId);
    
    //---------------------------------
    
    // CSV Recorder
    bool is_writing;
    wng::ofxCsv csv_recorder;
    bool is_reading;
    wng::ofxCsv csv_reader;
    int csv_row;
    int file_num;
    
    void WriteCsvData(wng::ofxCsv* csv_recorder);
    bool ReadCsvData();
};

#endif /* defined( __oFxGVFxPiano__gvfPianoInputs__) */


