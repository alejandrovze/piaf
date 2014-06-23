



//
//  gvfPianoHandler.h
//  ofgvfVisualiser
//
//  Created by Thomas Rushmore on 21/06/2013.
//  Modified by Igor Correa
//
//  An instance of this class should be used to access the gvf algorithm.
//  It provides methods to feed data to the particle filter and get the results from it.
//  It also provides methods to change the main gvf parameters.
//  For other useful functionalities, refer to the descriptions of the methods below.

//
//  gvfPianoHandler.h
//  oFxGVFxPiano
//
//  Created by Alejandro Van Zandt-Escobar on 12/12/2013.
//
//

#ifndef __oFxGVFxPiano__gvfPianoHandler__
#define __oFxGVFxPiano__gvfPianoHandler__

#include <iostream>
#include "ofMain.h"

#include "ofxGVF.h"
// #include "ofxGVFTypes.h"

//#include <Eigen/LU> // To use MatrixXf on overloaded gvf_data method
//#include "gvfGesture.h"

//using namespace Eigen;

// struct to hold information on how a learnt template is being recognised.
typedef struct {
    float likelihoods;
    float probability;
    float phase;
    float speed;
    vector<float> scale;
}  recognitionInfo;

class gvfPianoHandler{
public:
    
    gvfPianoHandler(int _inputDimension = 3);
    ~gvfPianoHandler();
    
    void setup(int _inputDimension);
    
    void init(); // Initialies the handler, depending on the input values.
    
    // Methods
    void gvf_learn();
    void gvf_follow();
    void gvf_clear();
    
    // feeds data to gvf based on the current state
    // argc is the number of dimentions on the input and argv is the input array
    void gvf_data(int argc, float *argv);
    void gvf_data(std::vector<float> p);
    
    // treats the 3 dimentional point p and calls gvf_data(int argc, float *argv);
    void gvf_data(ofPoint p);
    
    // returns a string containing information about the current recognition
    // (probabilities, phase, speed and scale for each gesture)
    string gvf_get_status();
    
    
    // Is it playing (receiving input)?
    bool getIsPlaying();
    bool toggleIsPlaying();
    
    //!!!: Some issues with recognition info, check out
    // vector containing one recognitionInfo struct for each template
    // these structs will contain real time information on how each template is being recognised
    std::vector<recognitionInfo> recogInfo;
    std::vector<recognitionInfo> getRecogInfo();
    recognitionInfo getTemplateRecogInfo(int templateNumber);
    float getProbability(int templateNumber);
    float getPhase(int templateNumber);
    recognitionInfo getRecogInfoOfMostProbable();
    
    void getGestureData();
    ofxGVFGesture* getCurrentGesture();
    
    void saveGestures(string fn);
    void loadGestures(string fn);
    
    
    ofxGVF* getGVF();
    
    void setState(ofxGVF::ofxGVFState state);

    
private:
    
    // reference to the gvf object
    ofxGVF              *mygvf;
    ofxGVFGesture       *currentGesture;
    ofxGVFConfig        gC;
    ofxGVFParameters    gP;
    ofxGVFOutcomes      gO;
    
    bool isPlaying;
    void startGesture();
    void endGesture();
    
    int inputDimension;
    int stateDimension;
//    ofxGVF::ofxGVFState state;
    
    
};

#endif /* defined(__oFxGVFxPiano__gvfPianoHandler__) */