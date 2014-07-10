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

typedef struct {
    float probability;
    float phase;
    float speed;
    vector<float> scale;
    vector<float> rotation;
}  Estimation;

class GVFHandler: public ofxGVF {
    
public:
    
    void init(int inputDimension);
    
    // feeds data to gvf based on the current state
    // argc is the number of dimentions on the input and argv is the input array
    void gvf_data(int argc, float *argv);
    void gvf_data(std::vector<float> p);
    
    // vector containing one recognitionInfo struct for each template
    // these structs will contain real time information on how each template is being recognised
    vector<Estimation> recogInfo;
    Estimation getTemplateRecogInfo(int templateNumber);
    Estimation getRecogInfoOfMostProbable(); // !!!: bad naming
    void UpdateRecogInfo();
    
    ofxGVFGesture* getCurrentGesture();
    
    void set_state(ofxGVF::ofxGVFState _state);
    
    bool getIsPlaying();
    bool toggleIsPlaying();    
    
private:
    
    ofxGVFGesture       *currentGesture;
    
    bool isPlaying;
    void startGesture();
    void endGesture();
    
    // Data dump
    
    // CSV Recorder
    wng::ofxCsv csv_recorder;
    void WriteCsvData(wng::ofxCsv* csv_recorder);
    
    
};

#endif /* defined(__piaf__GVFHandler__) */
