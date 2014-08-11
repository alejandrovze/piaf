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
#include "ofxUIMultiMovingGraph.h"

#include "gvfPianoInputs.h"
#include "GVFHandler.h"
#include "KinectDisplay.h"

#define KINECT_ON 0
#define ACC_ON 0
#define MOCAP_ON 1

class piafInterface {
    
public:
    
    void setup(GVFHandler* _gvf_handler, gvfPianoInputs* _inputs);
    void update();
    void draw();
    
private:
    
    GVFHandler* gvf_handler;
    gvfPianoInputs* inputs;
    
    int column_width;
    
    // INPUTS
	ofxUISuperCanvas* inputs_gui;
    void SetInputsGUI();
    void UpdateInputsGUI();
    void InputsGUIEvent(ofxUIEventArgs &e);
    
    bool acc_active;
    bool kinect_active;
    
    ofxUITextArea* gesture_length;
    vector<ofxUIMultiMovingGraph*> acc_inputs;
    vector<ofxUIMultiMovingGraph*> mocap_inputs;
    
    vector<ofxUILabelToggle*> mocap_toggles;
    vector<ofxUILabelToggle*> acc_toggles;
    ofxUIDropDownList* skeleton_list;
    
    bool kinect_display_on;
    KinectDisplay kinect_display;
    ofxUITextArea* kinect_status;
    
    // MARK: GVF STATUS
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
    
    // MARK: GVF SETTINGS
    ofxUISuperCanvas* settings_gui;
    void SetSettingsGUI();
    void UpdateSettingsGUI();
    void SettingsGUIEvent(ofxUIEventArgs &e);
    ofxUINumberDialer* tolerance;
    
    // TEMPLATES CANVAS
    ofxUISuperCanvas* templates_gui;
    void SetTemplatesGUI();
    void UpdateTemplatesGUI();
    void TemplatesGUIEvent(ofxUIEventArgs &e);
    void AddTemplate(int template_id, ofxGVFGesture &gesture);
    int n_templates;
    vector<ofxUIMinimalSlider* > phase_sliders;
    
    void SaveGestures();
    void LoadGestures();
    
};


#endif /* defined(__piaf__piafInterface__) */

