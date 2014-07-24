//
//  GVFHandler.h
//  piaf
//
//  Created by Alejandro Van Zandt-Escobar on 10/07/2014.
//
//

#ifndef __piaf__GVFHandler__
#define __piaf__GVFHandler__

#include <iostream>

#include "ofMain.h"
#include "ofxGVF.h"
#include "ofxCsv.h"

class GVFHandler: public ofxGVF {
    
public:
    
    void init(int inputDimension);
    void init(int inputDimension, vector<float> _min_range, vector<float> _max_range);
    
    // feeds data to gvf based on the current state
    // argc is the number of dimentions on the input and argv is the input array
    void gvf_data(int argc, float *argv);
    void gvf_data(std::vector<float> p);
    
    ofxGVFGesture* getCurrentGesture();
    
    void set_state(ofxGVF::ofxGVFState _state);
    
    bool getIsPlaying();
    bool toggleIsPlaying();
    
    void set_csv_path(string path);
    
private:
    
    ofxGVFGesture       *currentGesture;
    
    bool isPlaying;
    void startGesture();
    void endGesture();
    
    vector<float> min_range;
    vector<float> max_range;
    
    // Data dump
    
    // CSV Recorder
    string csv_path;
    int n_file;
    
    wng::ofxCsv csv_recorder;
    void SetupCsvData();
    void WriteCsvDataRow();
    void WriteCsvTemplates();
    void WriteCsvSettings();
    
};

#endif /* defined(__piaf__GVFHandler__) */
