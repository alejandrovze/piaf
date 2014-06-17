//
//  gvfPianoInterface.h
//  oFxGVFxPiano
//
//  Created by Alejandro Van Zandt-Escobar on 19/11/2013.
//
//  TODO: Link all values, implement a listener. 

#ifndef __oFxGVFxPiano__gvfPianoInterface__
#define __oFxGVFxPiano__gvfPianoInterface__

#include <iostream>

#include "gvfPianoApp.h"

#include "ofxGui.h"
#include "ofxUI.h"
#include "ofxUIMovingGraphX.h"

#include "KinectInput.h"

//--------------------------------------------------------------
// Extension of sliderGroup to display changing values.
template<class VecType>
class ofxVecSliderDisplay_ : public ofxVecSlider_<VecType> {
    
public:

    // This function is needed because changeValue is protected.
    void displayValue(VecType & value) {
        this->changeValue(value);
    }
    
};

typedef ofxVecSliderDisplay_<ofVec2f> ofxVec2SliderDisplay;
typedef ofxVecSliderDisplay_<ofVec3f> ofxVec3SliderDisplay;

//--------------------------------------------------------------
typedef struct {
    vector<vector<float> > data;
    ofxUIMovingGraphX* graph;
    ofxUISlider* phase_slider;
    float phase = 0;
    bool is_most_probable = false;
}   templateVisualiser;


//--------------------------------------------------------------
// GVF Piano Interface Class
class gvfPianoInterface {
	
public:
    
    // These are called by ofApp.
    void setup(gvfPianoHandler* _handler, gvfPianoInputs* _inputs, int _nAccelerometers, KinectInput* _kinect_input);
    void update();
    void draw();
    void exit();
    
private:
    
    // Reference to pianoApp allows interface to get information
    gvfPianoHandler* handler;
    gvfPianoInputs*  inputs;
    
    void drawString(); // ??? Instructions etc. Do we want this? 
    
    // --- SETTINGS GUI ---
    ofxPanel settingsGui;
    void initialiseSettingsGui();
    
    ofParameterGroup settings;
    ofParameter<int> numParticles;
    ofParameter<int> resampleThreshold;
    ofParameter<float> smoothingCoefficient;

    ofParameterGroup varianceCoefficients;
    ofParameter<float> sigPosition;
    ofParameter<float> sigRotation;
    ofParameter<float> sigScale;
    ofParameter<float> sigSpeed;
    
    void numParticlesChanged(int & numParticles);
    void resampleThresholdChanged(int & resampleThreshold);
    void smoothingCoefficientChanged(float & smoothingCoefficient);
    void varianceCoefficentsChanged(float & coefficent);
    
    ofxButton save;
    ofxButton load;
    
    void saveGestures();
    void loadGestures();
    
    // --- STATUS GUI ---
    ofxPanel statusGui;
    void initialiseStatusGui();
    void updateStatusGui(ofxGVF::ofxGVFState state, bool isFollowing, int mostProbable, recognitionInfo infoMostProbable);
    
    ofParameter<string> currentState;
    ofParameter<bool> inputOnOff;
    
    ofParameterGroup mostProbableStatus;
    ofParameter<int> mostProbableIndex;
    ofParameter<float> mostProbablePhase;
    ofParameter<float> mostProbableSpeed;
    ofParameter<float> mostProbableScale;
    
    // --- INPUTS GUI ---
    ofxPanel inputsGui;
    void initialiseInputsGui();
    void updateInputsGui(vector<float> inputData);
    
    vector<gvfInputInfo> inputsInfo;
    
    int nAccelerometers;
    vector<ofxVec3SliderDisplay> inputSliders; // FIXME: Generalize dimensions?
    ofxVec3SliderDisplay accSlider1;
    ofxVec3SliderDisplay accSlider2;
    ofxVec3SliderDisplay kinectSlider; 
    
    // --- GESTURE GUI ---
    
    ofxUICanvas* gesture_gui;
    ofxUIMovingGraphX* current_gesture_graph;
    void initialiseGestureGui(vector<vector<float> >* gesture_buffer);
    void updateGestureGui();
    
    
    // --- TEMPLATES GUI ---
    
    ofxUICanvas* templates_gui;
    void initialiseTemplatesGui();
    void updateTemplatesGui(int nTemplates);
    vector<templateVisualiser> templates;
    void updateTemplates(int nTemplates);
  
  
    // --- KINECT GUI ---
  
    KinectInput* kinect_input;
  
    void InitialiseKinectGui(KinectInput* _kinect_input);
    void UpdateKinectGui();
    
    ofImage depth_image;
    unsigned char *grayPixels;
  
    // Dimensions of frame in which to display Kinect.
    int kinect_width;
    int kinect_height;
    // Dimensions of original depth image.
    int depth_width;
    int depth_height;
  
    void UpdateDepth(openni::VideoFrameRef depth_frame);
    float* CalculateHistogram(int histogramSize, const openni::VideoFrameRef& depthFrame);
  
    void UpdateSkeleton(SkeletonDataPoint new_point);
    void DisplaySkeleton(SkeletonDataPoint new_point, ofPoint location);
  
};

#endif /* defined(__oFxGVFxPiano__gvfPianoInterface__) */
