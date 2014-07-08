//
//  gvfPianoHandler.h
//  oFxGVFxPiano
//
//  Created by Alejandro Van Zandt-Escobar on 12/12/2013.
//
//  An instance of this class should be used to access the gvf algorithm.
//  It provides methods to feed data to the particle filter and get the results from it.
//  It also provides methods to change the main gvf parameters.
//  For other useful functionalities, refer to the descriptions of the methods below.
//

#ifndef __oFxGVFxPiano__gvfPianoHandler__
#define __oFxGVFxPiano__gvfPianoHandler__

#include <iostream>

#include "ofMain.h"
#include "ofxGVF.h"
#include "ofxCsv.h"

// struct to hold information on how a learnt template is being recognised.
typedef struct {
    float probability;
    float phase;
    float speed;
    vector<float> scale;
    vector<float> rotation;
}  recognitionInfo;

class gvfPianoHandler{
    
public:
    
    gvfPianoHandler();
    ~gvfPianoHandler();
    
    void setup(int inputDimension);
    
    // feeds data to gvf based on the current state
    // argc is the number of dimentions on the input and argv is the input array
    void gvf_data(int argc, float *argv);
    void gvf_data(std::vector<float> p);
    
    // vector containing one recognitionInfo struct for each template
    // these structs will contain real time information on how each template is being recognised
    vector<recognitionInfo> recogInfo;
    recognitionInfo getTemplateRecogInfo(int templateNumber);
    recognitionInfo getRecogInfoOfMostProbable(); // !!!: bad naming
    void UpdateRecogInfo();
    
    ofxGVF* getGVF();
    ofxGVFGesture* getCurrentGesture();
    
    void setState(ofxGVF::ofxGVFState state);
    
    bool getIsPlaying();
    bool toggleIsPlaying();
    
private:
    
    // reference to the gvf object
    ofxGVF              *mygvf;
    ofxGVFGesture       *currentGesture;
    
    bool isPlaying;
    void startGesture();
    void endGesture();
    
    // Data dump
    
    // CSV Recorder
    wng::ofxCsv csv_recorder;
    void WriteCsvData(wng::ofxCsv* csv_recorder);
    
};

#endif /* defined(__oFxGVFxPiano__gvfPianoHandler__) */