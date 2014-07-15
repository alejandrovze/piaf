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
#include "WaxAccInput.h"
#include "PianoInput.h"

class gvfPianoInputs {
    
public:
    
    void setup();
    bool update();
    vector<float> initialise();

    vector<float> ConcatenateInput();
    int get_input_size();
    
    KinectInput* get_kinect_input();
    vector<WaxAccInput>* GetAccInputs();
    vector<ofPoint> GetKinectData();
    vector<ofVec3f> get_acc_data;
    
    vector<int> get_joints_on();
    void set_joints_on(vector<int> _joints_on);
    vector<int> get_acc_on();
    void set_acc_on(vector<int> _acc_on);
    
    ofEvent<ofxMidiMessage>& GetPianoEvent();
    ofEvent<ofxMidiMessage>& GetControlEvent();
    
    void StartFile();
    void EndFile();
    void LoadFile(string filename);
    
private:
    
    vector<bool> kinect_joints_on;
    int n_joints;
    
    // Kinect Input
    KinectInput kinect_input;
    vector<ofPoint> kinect_joints;
    vector<int> joints_on;
    
    // Accelerometers
    vector<WaxAccInput> accelerometers;
    vector<ofVec3f> acc_data;
    vector<int> acc_on;
    
    // Piano Input
    PianoInput piano;
    
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


