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

#include "MocapInput.h"
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
    vector<vector<float> > get_input_ranges();
    void ResetBounds();
    
    KinectInput* get_kinect_input();
    SkeletonDataPoint get_kinect_data();
    
    vector<ofVec3f> get_acc_data();
    vector<WaxAccInput*> GetAccInputs();
    
    int GetMocapSize();
    const vector<RigidBody>& get_mocap_data();
    const vector<ofVec3f>& get_mocap_bounds();
    
    
    vector<int> get_joints_on();
    void set_joints_on(vector<int> _joints_on);
    vector<int> get_acc_on();
    void set_acc_on(vector<int> _acc_on);
    vector<int> get_mocap_on();
    void set_mocap_on(vector<int> _mocap_on);
    
    ofEvent<ofxMidiMessage>& GetPianoEvent();
    ofEvent<ofxMidiMessage>& GetControlEvent();
    
    void StartFile();
    void EndFile();
    void LoadFile(string filename);
    void SetCsvPath(string path);
    
private:
    
    bool auto_ranges;

    // Mocap Input
    MocapInput mocap_input;
    vector<RigidBody> mocap_data;
    vector<int> mocap_on;
    
    // Kinect Input
    KinectInput kinect_input;
    SkeletonDataPoint kinect_data;
    vector<int> joints_on;
    
    // Accelerometers
    ofxOscReceiver wax_receiver;
    vector<WaxAccInput*> accelerometers;
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
    string csv_path;
    
    void WriteCsvData(wng::ofxCsv* csv_recorder);
    bool ReadCsvData();
};

#endif /* defined( __oFxGVFxPiano__gvfPianoInputs__) */


