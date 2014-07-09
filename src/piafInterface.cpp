//
//  piafInterface.cpp
//  piaf
//
//  Created by Alejandro Van Zandt-Escobar on 17/06/2014.
//
//

// TODO: Add Accelerometer Display
// TODO: Add current gesture data viz
// TODO: Add templates data viz

#include "piafInterface.h"

static const char * NiteJointStrings[] = {
	"NITE_JOINT_HEAD",
	"NITE_JOINT_NECK",
    
	"NITE_JOINT_LEFT_SHOULDER",
	"NITE_JOINT_RIGHT_SHOULDER",
	"NITE_JOINT_LEFT_ELBOW",
	"NITE_JOINT_RIGHT_ELBOW",
	"NITE_JOINT_LEFT_HAND",
	"NITE_JOINT_RIGHT_HAND",
    
	"NITE_JOINT_TORSO",
    
	"NITE_JOINT_LEFT_HIP",
	"NITE_JOINT_RIGHT_HIP",
	"NITE_JOINT_LEFT_KNEE",
	"NITE_JOINT_RIGHT_KNEE",
	"NITE_JOINT_LEFT_FOOT",
	"NITE_JOINT_RIGHT_FOOT"
};

//--------------------------------------------------------------
void piafInterface::setup(gvfPianoHandler* _handler, gvfPianoInputs* _inputs) {

    gvf_handler = _handler;
    inputs  = _inputs;
    
    kinect_image = inputs->get_kinect_input()->GetImage(); // TODO: Fix path to image.
    kinect_display = false;
    
    input_gesture = vector<ofxUIMovingGraph*>(inputs->getInputSize());
    buffers = vector<vector<float> >(inputs->getInputSize(), vector<float>(256, 0));
    
    initColors();
    
    ofSetCircleResolution(120);
    
    column_width = 212;
    
    SetInputsGUI();
    SetGvfGUI();
    SetSettingsGUI();
    SetTemplatesGUI();
    
}

//--------------------------------------------------------------
void piafInterface::draw(){
    
    ofBackground(255, 255, 255, 255);
    
    if (kinect_display)
        DrawKinectInterface(0, 0, ofGetWindowWidth(), ofGetWindowHeight());

    ofSetRectMode(OF_RECTMODE_CORNER);
    
}

//--------------------------------------------------------------
void piafInterface::update(){
    
    UpdateInputsGUI();
    UpdateGvfGUI();
    UpdateSettingsGUI();
    UpdateTemplatesGUI();

}

void piafInterface::exit() {
    
}

//--------------------------------------------------------------
//MARK: SECTION: Inputs
//--------------------------------------------------------------
void piafInterface::SetInputsGUI() {
    
    inputs_gui = new ofxUISuperCanvas("INPUTS");
    
    inputs_gui->addSpacer();
    inputs_gui->addLabel("KINECT");
    kinect_status = inputs_gui->addTextArea("textarea", "NULL KINECT", OFX_UI_FONT_SMALL);
    inputs_gui->addLabelToggle("KINECT_DISPLAY", false);
    
//    inputs_gui->addImage("kinect", kinect_image,
//                         kinect_image->getWidth() / 2.0,
//                         kinect_image->getHeight() / 2.0);
    
    // Currently deactivated Accelerometer interface.
    inputs_gui->addSpacer();
    inputs_gui->addLabel("ACCELEROMETER 1");
    inputs_gui->addLabelToggle("ACC1 ON/OFF", inputs->GetAccInputs()[0]);
    
    inputs_gui->addSpacer();
    
    inputs_gui->addLabel("INPUT GESTURE", OFX_UI_FONT_MEDIUM);
    for (int i = 0; i < input_gesture.size(); ++i) {
        input_gesture[i] = inputs_gui->addMovingGraph("MOVING", buffers[i], 256, 0.0, 1.0);
    }
    
    inputs_gui->addSpacer();
    
    vector<string> skeleton_items;
    for (int i = 0; i < NITE_JOINT_COUNT; ++i) {
        skeleton_items.push_back(NiteJointStrings[i]);
    }
    skeleton_list = inputs_gui->addDropDownList("JOINT SELECTION", skeleton_items);
    skeleton_list->setAllowMultiple(true);
    for (int i = 0; i < NITE_JOINT_COUNT; ++i) {
        skeleton_list->getToggles()[i]->setValue(inputs->GetKinectJoints()[i]);
    }
    
    
    inputs_gui->autoSizeToFitWidgets();
	ofAddListener(inputs_gui->newGUIEvent, this, &piafInterface::InputsGUIEvent);
    
}

void piafInterface::UpdateInputsGUI() {
    
    // Kinect Image
    if (kinect_display)
        inputs->get_kinect_input()->UpdateImage();
    
    switch(inputs->get_kinect_input()->get_state())
    {
        case nite::SKELETON_NONE:
            kinect_status->setTextString("NOT TRACKING");
            break;
        case nite::SKELETON_CALIBRATING:
            kinect_status->setTextString("CALIBRATING");
            break;
        case nite::SKELETON_TRACKED:
            kinect_status->setTextString("TRACKING");
            break;
        case nite::SKELETON_CALIBRATION_ERROR_NOT_IN_POSE:
        case nite::SKELETON_CALIBRATION_ERROR_HANDS:
        case nite::SKELETON_CALIBRATION_ERROR_LEGS:
        case nite::SKELETON_CALIBRATION_ERROR_HEAD:
        case nite::SKELETON_CALIBRATION_ERROR_TORSO:
            kinect_status->setTextString("ERROR");
            break;
    }
    
    vector<float> input_data = inputs->getInputData();
    
    //!!!: Quick fix for switching inputs. 
    if (input_data.size() == input_gesture.size()) {
        for (int i = 0; i < input_gesture.size(); ++i) {
            input_gesture[i]->addPoint(input_data[i]);
        }
    }
    
}

void piafInterface::InputsGUIEvent(ofxUIEventArgs &e) {
    
    string name = e.getName();
	int kind = e.getKind();
//	cout << "got event from: " << name << endl;
	
    if (kind == OFX_UI_WIDGET_LABELTOGGLE) {
        
        ofxUIToggle *toggle = (ofxUIToggle *) e.getToggle();
        
        if(name == "KINECT_DISPLAY")
        {
            kinect_display = toggle->getValue();
        }
    }
    
    if (name == "JOINT SELECTION" && (gvf_handler->getGVF()->getState() == ofxGVF::STATE_CLEAR)) {
        
        vector<int> selected = skeleton_list->getSelectedIndeces();
        
        inputs->SetKinectJoints(selected);

    }
}


//--------------------------------------------------------------
//MARK: SECTION: GVF Status
//--------------------------------------------------------------
void piafInterface::SetGvfGUI() {
    
    gvf_gui = new ofxUISuperCanvas("GVF Status");
    
    gvf_gui->addSpacer();
    gvf_gui->addFPS();
    
    gvf_gui->addSpacer();
    gvf_status = gvf_gui->addTextArea("textarea", "XXX", OFX_UI_FONT_SMALL);
    
    gvf_gui->addSpacer();
    is_playing = gvf_gui->addLabelToggle("PLAYING ON/OFF", false);
    
    gvf_gui->addSpacer();
    gvf_gui->setGlobalButtonDimension(32);
    gvf_gui->addButton("SAVE", false)->setLabelVisible(true);
    gvf_gui->addButton("LOAD", false)->setLabelVisible(true);
    
    gvf_gui->addSpacer();
    gvf_n_templates = gvf_gui->addTextArea("textarea", "NTEMPLATES 0", OFX_UI_FONT_SMALL);
    
    gvf_gui->addSpacer();
    gvf_gui->addLabel("Most Probable Template");
    gvf_most_probable = gvf_gui->addTextArea("textarea", "Index 0", OFX_UI_FONT_SMALL);
    gvf_most_probable_probability = gvf_gui->addTextArea("textarea", "Probability 0", OFX_UI_FONT_SMALL);
    gvf_most_probable_phase = gvf_gui->addTextArea("textarea", "Phase 0", OFX_UI_FONT_SMALL);
    gvf_most_probable_speed = gvf_gui->addTextArea("textarea", "Speed 0", OFX_UI_FONT_SMALL);
    gvf_most_probable_scale = gvf_gui->addTextArea("textarea", "Scale 0", OFX_UI_FONT_SMALL);
    gvf_most_probable_rotation = gvf_gui->addTextArea("textarea", "Rotation 0", OFX_UI_FONT_SMALL);
    
    gvf_gui->setPosition(column_width * 1, 0);
    gvf_gui->autoSizeToFitWidgets();
    
//    gvf_gui->addLabel("CURRENT GESTURE LENGTH", OFX_UI_FONT_SMALL);
//    gesture_length = gvf_gui->addTextArea("textarea", "0", OFX_UI_FONT_SMALL);
    
	ofAddListener(gvf_gui->newGUIEvent, this, &piafInterface::GvfGUIEvent);
}

void piafInterface::GvfGUIEvent(ofxUIEventArgs &e) {
    
    string name = e.getName();
	int kind = e.getKind();
	cout << "got event from: " << name << endl;
	
    if (kind == OFX_UI_WIDGET_BUTTON) {
        
        ofxUIButton *button = (ofxUIButton *) e.getButton();
        
        if(!button->getValue()) { // Execute on release
            if(name == "SAVE")
                SaveGestures();
            else if(name == "LOAD")
                LoadGestures();
        }
    }
    
}

void piafInterface::UpdateGvfGUI() {

    switch(gvf_handler->getGVF()->getState()) {
        case ofxGVF::STATE_WAIT:
            gvf_status->setTextString("STATE: Waiting");
            break;
        case ofxGVF::STATE_LEARNING:
            gvf_status->setTextString("STATE: Learning");
            break;
        case ofxGVF::STATE_FOLLOWING:
            gvf_status->setTextString("STATE: Following");
            break;
        case ofxGVF::STATE_CLEAR:
            gvf_status->setTextString("STATE: Clear");
            break;
    }
    
    is_playing->setValue(gvf_handler->getIsPlaying());
    
    gvf_n_templates->setTextString("NTEMPLATES " + ofToString(gvf_handler->getGVF()->getNumberOfGestureTemplates()));
    
    gvf_most_probable->setTextString("Index " +
                                     ofToString(gvf_handler->getGVF()->getMostProbableGestureIndex()));
    
    recognitionInfo status = gvf_handler->getRecogInfoOfMostProbable();
    gvf_most_probable_probability->setTextString("Probability " + ofToString(status.probability));
    gvf_most_probable_phase->setTextString("Phase " + ofToString(status.phase));
    gvf_most_probable_speed->setTextString("Speed " + ofToString(status.speed));
    
    string scale_string = "";
    for (int i = 0; i < status.scale.size(); ++i) {
        scale_string += ofToString(status.scale[i], 2);
        scale_string += " ";
    }
    gvf_most_probable_scale->setTextString("Scale " + scale_string);
    
    string rotation_string = "";
    for (int i = 0; i < status.rotation.size(); ++i) {
        rotation_string += ofToString(status.rotation[i], 2);
        rotation_string += " ";
    }
    gvf_most_probable_rotation->setTextString("Rotation " + rotation_string);
    
//    // Update Length of Gesture
//    if ((gvf_handler->getGVF()->getState() != ofxGVF::STATE_CLEAR) && gvf_handler->getIsPlaying()) {
//        gesture_length->setTextString(ofToString(gvf_handler->getCurrentGesture()->getTemplateLength()));
//    }
    
}


//--------------------------------------------------------------
//MARK: SECTION: GVF Settings
//--------------------------------------------------------------
void piafInterface::SetSettingsGUI() {
    
    settings_gui = new ofxUISuperCanvas("GVF Settings");
    
    // Parameters
    settings_gui->addNumberDialer("N Particles", 10, 10000, gvf_handler->getGVF()->getNumberOfParticles(), 0);
    settings_gui->addNumberDialer("Resampling Threshold", 100, 10000, gvf_handler->getGVF()->getResamplingThreshold(), 0);
    tolerance = settings_gui->addNumberDialer("Tolerance",  0.01, 1000.0, gvf_handler->getGVF()->getTolerance(), 3);
    settings_gui->addNumberDialer("Distribution", 0.0, 2.0, gvf_handler->getGVF()->getDistribution(), 2);
    
    // !!!: Variance Coefficients (currently crash)
    settings_gui->addNumberDialer("Phase Variance", 0.000001, 0.1, gvf_handler->getGVF()->getPhaseVariance(), 6);
    settings_gui->addNumberDialer("Speed Variance", 0.000001, 0.1, gvf_handler->getGVF()->getSpeedVariance(), 5);
    settings_gui->addNumberDialer("Scale Variance", 0.000001, 0.1, gvf_handler->getGVF()->getScaleVariance()[0], 5);
    settings_gui->addNumberDialer("Rotation Variance", 0.000001, 0.1, gvf_handler->getGVF()->getRotationVariance()[0], 5);

    settings_gui->setPosition(column_width * 2,0);
    settings_gui->autoSizeToFitWidgets();
    
	ofAddListener(settings_gui->newGUIEvent, this, &piafInterface::SettingsGUIEvent);
}

//--------------------------------------------------------------
void piafInterface::SettingsGUIEvent(ofxUIEventArgs &e) {
    
    string name = e.getName();
	int kind = e.getKind();
    
    if(kind == OFX_UI_WIDGET_NUMBERDIALER)
    {
        ofxUINumberDialer *n = (ofxUINumberDialer *) e.widget;
        float value = n->getValue();
        
        if (name == "N Particles") {
            gvf_handler->getGVF()->setNumberOfParticles((int) value);
        }
        else if (name == "Resampling Threshold") {
            gvf_handler->getGVF()->setResamplingThreshold((int) value);
        }
        else if (name == "Tolerance") {
            gvf_handler->getGVF()->setTolerance(value);
        }
        else if (name == "Distribution") {
            gvf_handler->getGVF()->setDistribution(value);
        }
        else if (name == "Phase Variance") {
            gvf_handler->getGVF()->setPhaseVariance(value);
        }
        else if (name == "Speed Variance") {
            gvf_handler->getGVF()->setSpeedVariance(value);
        }
        else if (name == "Scale Variance") {
            gvf_handler->getGVF()->setScaleVariance(value);
        }
        else if (name == "Rotation Variance") {
            gvf_handler->getGVF()->setRotationVariance(value);
        }
    }
    
}

//--------------------------------------------------------------
void piafInterface::UpdateSettingsGUI() {
    
    // Update Tolerance (dyamically set within GVF algorithm)
    tolerance->setValue(gvf_handler->getGVF()->getTolerance());
    
}

//--------------------------------------------------------------
//MARK: SECTION: TEMPLATES
//--------------------------------------------------------------
void piafInterface::SetTemplatesGUI() {
    
    templates_gui = new ofxUISuperCanvas("Templates");
    
    templates_gui->addSpacer();
    
    n_templates = 0;
    
    for (int id = 0; id < gvf_handler->getGVF()->getNumberOfGestureTemplates(); ++id) {
        AddTemplate(id, gvf_handler->getGVF()->getGestureTemplate(id));
    }
    
    
    templates_gui->setPosition(column_width * 3,0);
    templates_gui->autoSizeToFitWidgets();
    
	ofAddListener(templates_gui->newGUIEvent, this, &piafInterface::TemplatesGUIEvent);
    
    phase_sliders.clear();
}

//--------------------------------------------------------------
void piafInterface::UpdateTemplatesGUI() {
    
    int current_templates = gvf_handler->getGVF()->getNumberOfGestureTemplates();
    
    if (current_templates != n_templates) {
        delete templates_gui;
        SetTemplatesGUI();
    }
    
    // Update Phases
    for (int i = 0; i < n_templates; ++i) {
        
        if (gvf_handler->getGVF()->getState() == ofxGVF::STATE_FOLLOWING && gvf_handler->getIsPlaying()) {
            if (gvf_handler->getGVF()->getMostProbableGestureIndex() > -1) {
                phase_sliders[i]->setValue(gvf_handler->getGVF()->getOutcomes().allPhases[i]);
            
                float probability = gvf_handler->getGVF()->getOutcomes().allProbabilities[i];
                phase_sliders[i]->setColorFill(ofColor(255 * probability, 0, 0));
            }
            
            
        }
        else
            phase_sliders[i]->setColorFill(ofColor(255, 255, 255));
    }
    

}

//--------------------------------------------------------------
void piafInterface::AddTemplate(int template_id, ofxGVFGesture &gesture) {
    
    string label = "TEMPLATE " + ofToString(template_id);
    templates_gui->addLabel(label);
    
    // Display data contents.
    
    phase_sliders.push_back(templates_gui->addMinimalSlider("phase", 0., 1., 0.));
    
    templates_gui->addSpacer();
    
    ++n_templates;
    
    templates_gui->autoSizeToFitWidgets();
}

//--------------------------------------------------------------
void piafInterface::TemplatesGUIEvent(ofxUIEventArgs &e) {
    
    string name = e.getName();
	int kind = e.getKind();
    
}

// --------------------------------
//MARK: SECTION: Load/Save
// --------------------------------
//--------------------------------------------------------------
void piafInterface::SaveGestures() {

    string filename;

    ofFileDialogResult dialogResult = ofSystemSaveDialog("my_gestures", "Save gestures");
    if(!dialogResult.bSuccess)
        return;

    stringstream ss;
    ss << dialogResult.filePath;
    filename = ss.str();
    cout << filename;

    gvf_handler->getGVF()->saveTemplates(filename);
    cout << "Gestures saved.\n";
}

//--------------------------------------------------------------
void piafInterface::LoadGestures() {
    
    string filename;

    ofFileDialogResult dialogResult = ofSystemLoadDialog("Select the file containing gesture data");
    if(!dialogResult.bSuccess)
        return;

    stringstream ss;
    ss << dialogResult.filePath;
    filename = ss.str();
    cout << filename;

    gvf_handler->getGVF()->loadTemplates(filename);
    cout << "Gestures loaded.\n";
    
}

//--------------------------------------------------------------
//--------------------------------------------------------------
// MARK: Kinect
//--------------------------------------------------------------
//--------------------------------------------------------------

//--------------------------------------------------------------
//// Draw kinect templates and skeleton display
//--------------------------------------------------------------
void piafInterface::DrawKinectInterface(int x, int y, int width, int height) {
    
    if (inputs->get_kinect_input()->get_is_running()) {
        kinect_image->draw(x, y, width, height);
    }
    
//    if (inputs->get_kinect_input()->get_state() == nite::SKELETON_TRACKED)
    DrawSkeleton(inputs->GetKinectData(), x, y, width, height);
    
    if (gvf_handler->getCurrentGesture()->getNumberOfTemplates() > 0)
        DrawGesture(x, y, width, height);
    
    DrawTemplates(x, y, width, height);
    
}

void piafInterface::DrawGesture(int x, int y, int width, int height) {
        
    vector< vector<float> > template_data = gvf_handler->getCurrentGesture()->getTemplateRaw();
    vector<float> initial = gvf_handler->getCurrentGesture()->getInitialObservationRaw();
    
    int length = template_data.size();
    
    ofPolyline line;
    
    for (int t = 0; t < length; ++t) {
        
        int dimension = gvf_handler->getCurrentGesture()->getNumberDimensions();
        assert(dimension == 3);
        
        ofPoint orig_pos = ofPoint(template_data[t][0] + initial[0], template_data[t][1] + initial[1], template_data[t][2] + initial[2]);
        
        ofPoint pos = ScaleToKinect(orig_pos, x, y, width, height);
        
//        if (gvf_handler->getGVF()->getState() == ofxGVF::STATE_FOLLOWING && gvf_handler->getIsPlaying()) {
//            
//            // Set Color according to Probability
//            // FIXME: quick fix for getOutcomes crashing
//            float probability;
//            if (gvf_handler->getGVF()->getMostProbableGestureIndex() > -1) {
//                probability = gvf_handler->getGVF()->getOutcomes().allProbabilities[i];
//            }
//            else {
//                probability = 0;
//            }
//            ofColor template_color = ofColor(255, 0, 0) * probability + colors[i] * (1 - probability); // FIXME: unsafe
//            
//            ofSetColor(template_color);
//            
//            // Set Fill according to Phase
//            float phase = 0;
            
//            phase = gvf_handler->getGVF()->getOutcomes().allPhases[i];
//            
//            if (phase >= ((float) t / (float) length)) {
//                ofFill();
//            }
//            else {
//                ofNoFill();
//            }
//            
//        }
//        else {
//            ofSetColor(colors[i]);
//            ofNoFill();
//        }
//        // Fill according to phase reading.
        
        ofSetLineWidth(1);
        
        ofSetColor(ofColor(0, 255, 0));
        ofFill();
        
        line.addVertex(pos);
        ofCircle(pos, 5);
        
    }
    
    line.draw();
    
    
    ofSetColor(255, 255, 255);
    ofNoFill();

    
}

//--------------------------------------------------------------
//// Draw each saved template, with phase if following
//--------------------------------------------------------------
void piafInterface::DrawTemplates(int x, int y, int width, int height) {
    
    int n_templates = gvf_handler->getGVF()->getNumberOfGestureTemplates();
    
    for (int i = 0; i < n_templates; ++i) {
        
        vector< vector<float> > template_data = gvf_handler->getGVF()->getGestureTemplate(i).getTemplateRaw();
        vector<float> initial = gvf_handler->getGVF()->getGestureTemplate(i).getInitialObservationRaw();
        
        int length = template_data.size();
        
        ofPolyline line;
        
        for (int t = 0; t < length; ++t) {
            
            int dimension = gvf_handler->getGVF()->getGestureTemplate(i).getNumberDimensions();
            assert(dimension == 3);
            
            ofPoint orig_pos = ofPoint(template_data[t][0] + initial[0], template_data[t][1] + initial[1], template_data[t][2] + initial[2]);
            
            ofPoint pos = ScaleToKinect(orig_pos, x, y, width, height);
            
            if (gvf_handler->getGVF()->getState() == ofxGVF::STATE_FOLLOWING && gvf_handler->getIsPlaying()) {
                
                // Set Color according to Probability
                // FIXME: quick fix for getOutcomes crashing
                float probability;
                if (gvf_handler->getGVF()->getMostProbableGestureIndex() > -1) {
                    probability = gvf_handler->getGVF()->getOutcomes().allProbabilities[i];
                }
                else {
                    probability = 0;
                }
                ofColor template_color = ofColor(255, 0, 0) * probability + colors[i] * (1 - probability); // FIXME: unsafe
                
                ofSetColor(template_color);
                
                ofSetLineWidth(0.5 + 10 * probability);
                
                // Set Fill according to Phase
                float phase = 0;
                
                phase = gvf_handler->getGVF()->getOutcomes().allPhases[i];
                
                if (phase >= ((float) t / (float) length)) {
                    ofFill();
                }
                else {
                    ofNoFill();
                }
                
            }
            else {
                
                ofSetLineWidth(1);
                ofSetColor(colors[i]);
                ofNoFill();
            }
            // Fill according to phase reading.
            
            line.addVertex(pos);
            ofCircle(pos, 5);
            
        }
        line.draw();
        
        
        ofSetColor(255, 255, 255);
        ofNoFill();
        
        
        
    }
    
    
}

//--------------------------------------------------------------
//// Live skeleton display
//--------------------------------------------------------------
void piafInterface::DrawSkeleton(vector<ofPoint> skeleton, int x, int y, int width, int height) {
    
    int x_pos;
    int y_pos;
    
    ofFill();
    
    
    ofSetColor(0, 0, 255);
    
    
    for (int i = 0; i <= NITE_JOINT_TORSO; ++i) {
        
        ofPoint depth_pt = ScaleToKinect(skeleton[i], x, y, width, height);
        
        ofCircle(depth_pt, 5);
        
    }
    
    ofNoFill();
    
    ofSetColor(255, 255, 255);
    
}

ofPoint piafInterface::ScaleToKinect(ofPoint init_point, int x, int y, int width, int height) {
    
    ofPoint point;
    
    if (inputs->get_kinect_input()->get_is_running()) {
        
        // FIXME: Convert World to Depth without Kinect plugged in?
        point = inputs->get_kinect_input()->convert_world_to_depth(init_point);
        
        point.x = x + (int) ((float) (point.x) * (float) width / (float) kinect_image->getWidth());
        point.y = y + (int) ((float) (point.y) * (float) height / (float) kinect_image->getHeight());
        
         
        
        return point;
    }
    
    else {
        // TODO: how to deal with this?
        return init_point;
    }

}



// COLORS
//--------------------------------------------------------------
void piafInterface::initColors()
{
    colors.clear();
    colors.push_back(ofColor::blue);
    colors.push_back(ofColor::cyan);
    colors.push_back(ofColor::aqua);
    colors.push_back(ofColor::violet);
    colors.push_back(ofColor::aliceBlue);
    colors.push_back(ofColor::aquamarine);
    colors.push_back(ofColor::azure);
}

//--------------------------------------------------------------
ofColor piafInterface::generateRandomColor()
{
    ofColor c;
    
    if(colors.size() == 0)
        initColors();
    
    int colorsRemaining = colors.size();
    
    int index = ofRandom(0, colorsRemaining - 1);
    
    c = colors[index];
    colors.erase(colors.begin() + index);
    return c;
}



