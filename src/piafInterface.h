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

class piafInterface {
    
public:
    
    void setup(gvfPianoHandler* _gvf_handler, gvfPianoInputs* _inputs);
    void update();
    void draw();
    void exit();
    
private:
    
    gvfPianoHandler* gvf_handler;
    gvfPianoInputs* inputs;
    
    int column_width;
    
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
    
    // GVF Status
    ofxUISuperCanvas* gvf_gui;
    void SetGvfGUI();
    void UpdateGvfGUI();
    void GvfGUIEvent(ofxUIEventArgs &e);
    ofxUITextArea* gvf_status;
    ofxUITextArea* gvf_n_templates;
    ofxUITextArea* gvf_most_probable;
    ofxUITextArea* gvf_most_probable_probability;
    ofxUITextArea* gvf_most_probable_phase;
    ofxUITextArea* gvf_most_probable_speed;
    ofxUITextArea* gvf_most_probable_scale;
    ofxUITextArea* gvf_most_probable_rotation;
    ofxUIToggle* is_playing;
    
    // GVF Settings
    ofxUISuperCanvas* settings_gui;
    void SetSettingsGUI();
    void UpdateSettingsGUI();
    void SettingsGUIEvent(ofxUIEventArgs &e);
    ofxUINumberDialer* tolerance;
    
    // TEMPLATE
    // TODO: How to visualize templates?
    
    
    void SaveGestures();
    void LoadGestures();
    
};


#endif /* defined(__piaf__piafInterface__) */
    



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

