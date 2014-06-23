//
//  piafInterface.h
//  piaf
//
//  Created by Alejandro Van Zandt-Escobar on 17/06/2014.
//
//

#ifndef __piaf__piafInterface__
#define __piaf__piafInterface__

#include <iostream>

#include "ofxUI.h"

#include "gvfPianoInputs.h"
#include "gvfPianoHandler.h"
#include "gvfPianoApp.h"

class piafInterface {
    
public:
    
    void setup(gvfPianoHandler* _gvf_handler, gvfPianoInputs* _inputs);
    void update();
    void draw();
    void exit();
    
private:
    
    gvfPianoHandler* gvf_handler;
    gvfPianoInputs* inputs;
    
    // ???: Add Instructions string
    
    // INPUTS
	ofxUISuperCanvas* inputs_gui;
    void SetInputsGUI();
    void UpdateInputsGUI();
    void InputsGUIEvent(ofxUIEventArgs &e);
    ofImage* kinect_image;
    ofxUITextArea* kinect_status;
    bool kinect_display;
    ofxUITextArea* gesture_length;
    vector<ofxUIMovingGraph*> input_gesture;
    vector<vector<float> > buffers;
    //    void UpdateSkeleton(SkeletonDataPoint new_point);
    //    void DisplaySkeleton(SkeletonDataPoint new_point, ofPoint location);

    
    // GVF
    ofxUISuperCanvas* gvf_gui;
    void SetGvfGUI();
    void UpdateGvfGUI();
    void GvfGUIEvent(ofxUIEventArgs &e);
    ofxUITextArea* gvf_status;
    ofxUITextArea* gvf_n_templates;
    ofxUITextArea* gvf_most_probable;
    ofxUITextArea* gvf_most_probable_phase;
    ofxUIToggle* is_playing;
    
    void SaveGestures();
    void LoadGestures();
    
    // TEMPLATE
    // TODO: How to visualize templates?
    
    
};


#endif /* defined(__piaf__piafInterface__) */
    

// TODO: GVF Parameters

//    ofParameterGroup settings;
//    ofParameter<int> numParticles;
//    ofParameter<int> resampleThreshold;
//    ofParameter<float> smoothingCoefficient;
//    
//    ofParameterGroup varianceCoefficients;
//    ofParameter<float> sigPosition;
//    ofParameter<float> sigRotation;
//    ofParameter<float> sigScale;
//    ofParameter<float> sigSpeed;
//    
//    void numParticlesChanged(int & numParticles);
//    void resampleThresholdChanged(int & resampleThreshold);
//    void smoothingCoefficientChanged(float & smoothingCoefficient);
//    void varianceCoefficentsChanged(float & coefficent);
//    
//    ofParameterGroup mostProbableStatus;
//    ofParameter<int> mostProbableIndex;
//    ofParameter<float> mostProbablePhase;
//    ofParameter<float> mostProbableSpeed;
//    ofParameter<float> mostProbableScale;


//    // --- GESTURE GUI ---
//    
//    ofxUICanvas* gesture_gui;
//    ofxUIMovingGraphX* current_gesture_graph;
//    void initialiseGestureGui(vector<vector<float> >* gesture_buffer);
//    void updateGestureGui();
//    
//    // --- TEMPLATES GUI ---
//    
//    ofxUICanvas* templates_gui;
//    void initialiseTemplatesGui();
//    void updateTemplatesGui(int nTemplates);
//    vector<templateVisualiser> templates;
//    void updateTemplates(int nTemplates);
//    

