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
    
    ofxUITextArea* gesture_length;
    vector<ofxUIMovingGraph*> input_gesture;
    vector<vector<float> > buffers;
    
    ofxUIDropDownList* skeleton_list;
    
    void DrawKinectInterface(int x, int y, int width, int height);
    ofImage* kinect_image;
    ofxUITextArea* kinect_status;
    bool kinect_display;
    void DrawSkeleton(vector<ofPoint> skeleton, int x, int y, int width, int height);
    void DrawTemplates(int x, int y, int width, int height);
    void DrawGesture(int x, int y, int width, int height);
    ofPoint ScaleToKinect(ofPoint init_point, int x, int y, int width, int height);
    
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
    
    // COLORS
    void initColors();
    ofColor generateRandomColor();
    std::vector<ofColor> colors;
    
};


#endif /* defined(__piaf__piafInterface__) */

