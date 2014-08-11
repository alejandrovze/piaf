//
//  piafInterface.cpp
//  piaf
//
//  Created by Alejandro Van Zandt-Escobar on 17/06/2014.
//
//

#include "piafInterface.h"

//--------------------------------------------------------------
void piafInterface::setup(GVFHandler* _handler, gvfPianoInputs* _inputs) {

    gvf_handler = _handler;
    inputs  = _inputs;
    
    kinect_display.setup(inputs->get_kinect_input(), gvf_handler);
    
    kinect_display_on = false;
    
    ofSetCircleResolution(120);
    
    column_width = 212;
    
    SetInputsGUI();
    SetGvfGUI();
    SetSettingsGUI();
    SetTemplatesGUI();
    
    acc_active = false;
    kinect_active = false;
    
}

//--------------------------------------------------------------
void piafInterface::draw() {
    
    ofSetRectMode(OF_RECTMODE_CORNER);
        
    ofPushMatrix();
    
//    SkeletonDataPoint skeleton = inputs->get_kinect_data();
//    
//    if (kinect_display_on) {
//        kinect_display.DrawKinect(skeleton);
//    }
//    else {
//        kinect_display.Draw(skeleton, inputs->get_joints_on());
//    }
    
    ofPopMatrix();
    
}

//--------------------------------------------------------------
void piafInterface::update(){
    
    UpdateInputsGUI();
    UpdateGvfGUI();
    UpdateSettingsGUI();
    UpdateTemplatesGUI();

}

//--------------------------------------------------------------
//MARK: SECTION: Inputs
//--------------------------------------------------------------
void piafInterface::SetInputsGUI() {
    
    inputs_gui = new ofxUISuperCanvas("INPUTS");
    
#if KINECT_ON
        inputs_gui->addSpacer();
        inputs_gui->addLabel("KINECT");
        kinect_status = inputs_gui->addTextArea("textarea", "NULL KINECT", OFX_UI_FONT_SMALL);
        inputs_gui->addLabelToggle("KINECT_DISPLAY", kinect_display_on);
    
    // Potentially display Kinect image in menu.
    //    inputs_gui->addImage("kinect", kinect_image,
    //                         kinect_image->getWidth() / 2.0,
    //                         kinect_image->getHeight() / 2.0);
    
#endif
    
    inputs_gui->addSpacer();
    
#if MOCAP_ON
    // Mocap Inputs
    for (int i = 0; i < inputs->GetMocapSize(); ++i) {
        inputs_gui->addSpacer();
        
        string mocap_name = "RIGID BODY " + ofToString(i);
        mocap_toggles.push_back(inputs_gui->addLabelToggle(mocap_name, false));
        
        mocap_inputs.push_back(ofxUIMultiMovingGraph::addToCanvas(inputs_gui, "MOVING", vector<ofVec3f>(256), 256, -500.0, 500.0));
        mocap_inputs[i]->setColorFill(ofColor::blue);
    }
    for (int i = 0; i < inputs->get_mocap_on().size(); ++i) {
        mocap_toggles[inputs->get_mocap_on()[i]]->setValue(true);
    }
#endif
    
    inputs_gui->addSpacer();
    
#if ACC_ON
    if (acc_active) {
        // Accelerometer Inputs
        for (int i = 0; i < inputs->GetAccInputs().size(); ++i) {
            inputs_gui->addSpacer();
            
            string acc_name = "ACCELEROMETER " + ofToString(inputs->GetAccInputs()[i]->get_id());
            acc_toggles.push_back(inputs_gui->addLabelToggle(acc_name, false));
            
            acc_inputs.push_back(ofxUIMultiMovingGraph::addToCanvas(inputs_gui, "MOVING", vector<ofVec3f>(256), 256, -1.0, 1.0));
            acc_inputs[i]->setColorFill(ofColor::yellow);
        }
    }
#endif
    
    inputs_gui->addSpacer();
    
    if (kinect_active) {
        vector<string> skeleton_items;
        for (int i = 0; i < NITE_JOINT_COUNT; ++i) {
            skeleton_items.push_back(NiteJointStrings[i]);
        }
        skeleton_list = inputs_gui->addDropDownList("JOINT SELECTION", skeleton_items);
        skeleton_list->setAllowMultiple(true);
        for (int i = 0; i < inputs->get_joints_on().size(); ++i) {
            skeleton_list->getToggles()[inputs->get_joints_on()[i]]->setValue(true);
        }
    }
    
    // TODO: Add settings for ranges of inputs. 
    
    inputs_gui->addButton("RESET INPUTS", true);
    inputs_gui->addButton("RESET BOUNDS", true);
    
    inputs_gui->autoSizeToFitWidgets();
	ofAddListener(inputs_gui->newGUIEvent, this, &piafInterface::InputsGUIEvent);
    
}

//--------------------------------------------------------------
void piafInterface::UpdateInputsGUI() {
    
#if KINECT_ON
    // Kinect Image
    if (kinect_display_on)
        inputs->get_kinect_input()->UpdateImage();
    
    switch(inputs->get_kinect_input()->get_data().state)
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
#endif
    
#if ACC_ON
    if (acc_active) {
        for (int i = 0; i < inputs->GetAccInputs().size(); ++i)
            acc_inputs[i]->addPoint(inputs->get_acc_data()[i]);
    }
#endif
    
#if MOCAP_ON
    for (int i = 0; i < inputs->GetMocapSize(); ++i)
        mocap_inputs[i]->addPoint(inputs->get_mocap_data()[i].position);
#endif
    
}

//--------------------------------------------------------------
void piafInterface::InputsGUIEvent(ofxUIEventArgs &e) {
    
    string name = e.getName();
	int kind = e.getKind();
	
    if (kind == OFX_UI_WIDGET_LABELTOGGLE) {
        
        ofxUIToggle *toggle = (ofxUIToggle *) e.getToggle();
        
        if(name == "KINECT_DISPLAY")
        {
            kinect_display_on = toggle->getValue();
        }
    }
    
    if (name == "RESET BOUNDS") {
        inputs->ResetBounds();
    }
    
    if (name == "RESET INPUTS") {
        if(gvf_handler->getState() == ofxGVF::STATE_CLEAR) {

#if KINECT_ON
            inputs->set_joints_on(skeleton_list->getSelectedIndeces());
#endif
            
            vector<int> acc_on;
            for (int i = 0; i < acc_toggles.size(); ++i) {
                if (acc_toggles[i]->getValue())
                    acc_on.push_back(i);
            }
            inputs->set_acc_on(acc_on);
            
            vector<int> mocap_on;
            for (int i = 0; i < mocap_toggles.size(); ++i) {
                if (mocap_toggles[i]->getValue())
                    mocap_on.push_back(i);
            }
            inputs->set_mocap_on(mocap_on);
            
            // Re-initialise GVF
            gvf_handler->init(inputs->get_input_size(), inputs->get_input_ranges()[0], inputs->get_input_ranges()[1]);
            
            // Scaling display (hasty to be improved)
            for (int i = 0; i < inputs->GetMocapSize(); ++i) {
                
                ofVec3f min_range = inputs->get_mocap_bounds()[2 * i + 0];
                ofVec3f max_range = inputs->get_mocap_bounds()[2 * i + 1];
                
                float min = min_range[0];
                float max = max_range[0];
                
                for (int i = 1; i < 3; ++i) {
                    if (min > min_range[i])
                        min = min_range[i];
                    if (max < max_range[i])
                        max = max_range[i];
                }
                
                mocap_inputs[i]->setMaxAndMin(max, min);
            }
            
            // Refresh interface
            delete gvf_gui;
            delete settings_gui;
            
            SetSettingsGUI();
            SetGvfGUI();
            
        }
        else
            cout << "Reset Only available in STATE_CLEAR" << endl;
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
    gvf_gui->addButton("SET DUMP FOLDER", false)->setLabelVisible(true);
    gvf_gui->addButton("SET GESTURE FOLDER", false)->setLabelVisible(true);
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
    
    gvf_gui->addSpacer();
    
    gesture_length = gvf_gui->addTextArea("GESTURE_LENGTH", "GESTURE LENGTH 0", OFX_UI_FONT_SMALL);
    
    gvf_gui->setPosition(column_width * 1, 0);
    gvf_gui->autoSizeToFitWidgets();
    
	ofAddListener(gvf_gui->newGUIEvent, this, &piafInterface::GvfGUIEvent);
}

//--------------------------------------------------------------
void piafInterface::GvfGUIEvent(ofxUIEventArgs &e) {
    
    string name = e.getName();
	int kind = e.getKind();
	
    if (kind == OFX_UI_WIDGET_BUTTON) {
        
        ofxUIButton *button = (ofxUIButton *) e.getButton();
        
        if(!button->getValue()) { // Execute on release
            if(name == "SAVE") {
                SaveGestures();
            }
            else if(name == "LOAD") {
                LoadGestures();
            }
            else if(name == "SET DUMP FOLDER") {
                
                string path;
                
                ofFileDialogResult dialogResult = ofSystemSaveDialog("gvf_dump-", "Select dump location.");
                if(!dialogResult.bSuccess)
                    return;
                
                dialogResult.getPath();
                
                stringstream ss;
                ss << dialogResult.filePath;
                path = ss.str();
                cout << path;
                
                gvf_handler->set_csv_path(path);
                
            }
            else if(name == "SET GESTURE FOLDER") {
                
                string path;
                
                ofFileDialogResult dialogResult = ofSystemSaveDialog("gvf_gesture-", "Select gesture location.");
                if(!dialogResult.bSuccess)
                    return;
                
                dialogResult.getPath();
                
                stringstream ss;
                ss << dialogResult.filePath;
                path = ss.str();
                cout << path;
                
                inputs->SetCsvPath(path);
                
            }
        }
    }
    else if (kind == OFX_UI_WIDGET_LABELTOGGLE) {
        ofxUIToggle *toggle = (ofxUIToggle *) e.getToggle();
        
        if (name == "PLAYING ON/OFF") {
            gvf_handler->toggleIsPlaying();
        }
    }
    
}

//--------------------------------------------------------------
void piafInterface::UpdateGvfGUI() {

    switch(gvf_handler->getState()) {
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
    
    gvf_n_templates->setTextString("NTEMPLATES " + ofToString(gvf_handler->getNumberOfGestureTemplates()));
    
    gvf_most_probable->setTextString("Index " +
                                     ofToString(gvf_handler->getMostProbableGestureIndex()));
    
    ofxGVFEstimation status = gvf_handler->getRecogInfoOfMostProbable();
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
    
    if ((gvf_handler->getState() != ofxGVF::STATE_CLEAR) && gvf_handler->getIsPlaying()) {
        string text_string = "GESTURE LENGTH " + ofToString(gvf_handler->getCurrentGesture()->getTemplateLength());
        gesture_length->setTextString(text_string);
    }
    
}


//--------------------------------------------------------------
//MARK: SECTION: GVF Settings
//--------------------------------------------------------------
void piafInterface::SetSettingsGUI() {
    
    settings_gui = new ofxUISuperCanvas("GVF Settings");
    
    // Parameters
//    settings_gui->addNumberDialer("N Particles", 10, 10000, gvf_handler->getNumberOfParticles(), 0);
//    settings_gui->addNumberDialer("Resampling Threshold", 100, 10000, gvf_handler->getResamplingThreshold(), 0);
    tolerance = settings_gui->addNumberDialer("Tolerance",  0.01, 1000.0, gvf_handler->getTolerance(), 3);
    settings_gui->addNumberDialer("Distribution", 0.0, 2.0, gvf_handler->getDistribution(), 2);
    
    // !!!: Variance Coefficients (currently crash)
    settings_gui->addNumberDialer("Phase Variance", 0.000001, 0.1, gvf_handler->getPhaseVariance(), 6);
    settings_gui->addNumberDialer("Speed Variance", 0.000001, 0.1, gvf_handler->getSpeedVariance(), 5);
    for (int i = 0; i < gvf_handler->getScaleVariance().size(); ++i) {
        settings_gui->addNumberDialer("Scale Variance" + ofToString(i), 0.000001, 0.1, gvf_handler->getScaleVariance()[i], 5);
    }
    for (int i = 0; i < gvf_handler->getRotationVariance().size(); ++i) {
        settings_gui->addNumberDialer("Rotation Variance" + ofToString(i), 0.000001, 0.1, gvf_handler->getRotationVariance()[i], 5);
    }

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
            gvf_handler->setNumberOfParticles((int) value);
        }
        else if (name == "Resampling Threshold") {
            gvf_handler->setResamplingThreshold((int) value);
        }
        else if (name == "Tolerance") {
            gvf_handler->setTolerance(value);
        }
        else if (name == "Distribution") {
            gvf_handler->setDistribution(value);
        }
        else if (name == "Phase Variance") {
            gvf_handler->setPhaseVariance(value);
        }
        else if (name == "Speed Variance") {
            gvf_handler->setSpeedVariance(value);
        }
        else if (name == "Scale Variance0") {
            gvf_handler->setScaleVariance(value, 0);
        }
        else if (name == "Scale Variance1") {
            gvf_handler->setScaleVariance(value, 1);
        }
        else if (name == "Scale Variance2") {
            gvf_handler->setScaleVariance(value, 2);
        }
        else if (name == "Rotation Variance0") {
            gvf_handler->setRotationVariance(value, 0);
        }
        else if (name == "Rotation Variance1") {
            gvf_handler->setRotationVariance(value, 1);
        }
        else if (name == "Rotation Variance2") {
            gvf_handler->setRotationVariance(value, 2);
        }
    }
    
}

//--------------------------------------------------------------
void piafInterface::UpdateSettingsGUI() {
    
    // Update Tolerance (dyamically set within GVF algorithm)
    tolerance->setValue(gvf_handler->getTolerance());
    
}

//--------------------------------------------------------------
//MARK: SECTION: TEMPLATES
//--------------------------------------------------------------
void piafInterface::SetTemplatesGUI() {
    
    templates_gui = new ofxUISuperCanvas("Templates");
    
    templates_gui->addSpacer();
    
    n_templates = 0;
    
    for (int id = 0; id < gvf_handler->getNumberOfGestureTemplates(); ++id) {
        AddTemplate(id, gvf_handler->getGestureTemplate(id));
    }
    
    
    templates_gui->setPosition(column_width * 3,0);
    templates_gui->autoSizeToFitWidgets();
    
	ofAddListener(templates_gui->newGUIEvent, this, &piafInterface::TemplatesGUIEvent);
    
    phase_sliders.clear();
}

//--------------------------------------------------------------
void piafInterface::UpdateTemplatesGUI() {
    
    int current_templates = gvf_handler->getNumberOfGestureTemplates();
    
    if (current_templates != n_templates) {
        delete templates_gui;
        SetTemplatesGUI();
    }
    
    // Update Phases
    for (int i = 0; i < n_templates; ++i) {
        
        if (gvf_handler->getState() == ofxGVF::STATE_FOLLOWING && gvf_handler->getIsPlaying()) {
            if (gvf_handler->getMostProbableGestureIndex() > -1) {
                phase_sliders[i]->setValue(gvf_handler->getOutcomes().estimations[i].phase);
            
                float probability = gvf_handler->getOutcomes().estimations[i].probability;
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

    gvf_handler->saveTemplates(filename);
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

    gvf_handler->loadTemplates(filename);
    cout << "Gestures loaded.\n";
    
}





