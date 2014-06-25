//
//  piafInterface.cpp
//  piaf
//
//  Created by Alejandro Van Zandt-Escobar on 17/06/2014.
//
//

// TODO: Figure out ofxGVF variances problem.
// TODO: Add Kinect Display
// TODO: Add Accelerometer Display
// TODO: Add current gesture data viz
// TODO: Add templates data viz

#include "piafInterface.h"

//--------------------------------------------------------------
void piafInterface::setup(gvfPianoHandler* _handler, gvfPianoInputs* _inputs) {

    gvf_handler = _handler;
    inputs  = _inputs;
    
    kinect_image = inputs->get_kinect_input()->GetImage(); // TODO: Fix path to image.
    kinect_display = false;
    
    input_gesture = vector<ofxUIMovingGraph*>(inputs->getInputSize());
    buffers = vector<vector<float> >(inputs->getInputSize(), vector<float>(256, 0));
    
    ofSetCircleResolution(120);
    
    column_width = 212;
    
    SetInputsGUI();
    SetGvfGUI();
    SetSettingsGUI();
    
}

//--------------------------------------------------------------
void piafInterface::draw(){
    
    ofBackground(255, 255, 255, 255);
    
    if (kinect_display)
        kinect_image->draw(0, 0, 1200, 800); // Ideally, within a GUI canvas
    
    ofSetRectMode(OF_RECTMODE_CENTER);
    
}

//--------------------------------------------------------------
void piafInterface::update(){
    
    UpdateInputsGUI();
    UpdateGvfGUI();
    UpdateSettingsGUI();

}

void piafInterface::exit() {
    
}

//--------------------------------------------------------------
//MARK: Inputs
//--------------------------------------------------------------
void piafInterface::SetInputsGUI() {
    
    inputs_gui = new ofxUISuperCanvas("INPUTS");
    
    inputs_gui->addSpacer();
    inputs_gui->addLabel("KINECT");
    kinect_status = inputs_gui->addTextArea("textarea", "NULL KINECT", OFX_UI_FONT_SMALL);
    inputs_gui->addLabelToggle("KINECT_DISPLAY", true);

    // TODO: Get moving image to work for Kinect visualization
//    inputs_gui->addSpacer();
//	inputs_gui->addImage("KINECT DISPLAY", kinect_image);
    
    inputs_gui->addSpacer();
    inputs_gui->addLabel("ACCELEROMETER 1");
    inputs_gui->addLabelToggle("ACC1 ON/OFF", true);
    inputs_gui->addSpacer();
    inputs_gui->addLabel("ACC1 WAX ID");
    inputs_gui->addNumberDialer("ACC1", 0, 20, 12, 1);
    
    // TODO: Add Moving Graph for each ACC Dimension
    
    inputs_gui->addSpacer();
    inputs_gui->addLabel("ACCELEROMETER 2");
    inputs_gui->addLabelToggle("ACC2ON/OFF", true);
    inputs_gui->addSpacer();
    inputs_gui->addLabel("ACC2 WAX ID");
    inputs_gui->addNumberDialer("ACC2", 0, 20, 14, 1);
    
    inputs_gui->addLabel("INPUT GESTURE", OFX_UI_FONT_MEDIUM);
    for (int i = 0; i < input_gesture.size(); ++i) {
        input_gesture[i] = inputs_gui->addMovingGraph("MOVING", buffers[i], 256, 0.0, 1.0);
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
    for (int i = 0; i < input_gesture.size(); ++i) {
        input_gesture[i]->addPoint(input_data[i]);
    }
    
}

void piafInterface::InputsGUIEvent(ofxUIEventArgs &e) {
    
    string name = e.getName();
	int kind = e.getKind();
	cout << "got event from: " << name << endl;
	
    if (kind == OFX_UI_WIDGET_LABELTOGGLE) {
        
        ofxUIToggle *toggle = (ofxUIToggle *) e.getToggle();
        
        if(name == "KINECT_DISPLAY")
        {
            kinect_display = toggle->getValue();
        }
//        else if (name == "ACC1 ON/OFF") {
//            inputs->setInputs
//        }
    }
}


//--------------------------------------------------------------
//---------------GVF---------------------------------------
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
    
    // !!!: Temp: Feedback for recognition
//    if (gvf_handler->getIsPlaying() && (gvf_handler->getGVF()->getState() == ofxGVF::STATE_FOLLOWING)) {
//    
//        cout << "INDEX " << ofToString(gvf_handler->getGVF()->getMostProbableGestureIndex());
//        cout << " P " << ofToString(status.probability);
//        cout << " Ph " << ofToString(status.phase);
//        cout << " Sp " << ofToString(status.speed);
//        for (int i = 0; i < status.scale.size(); ++i)
//            cout << " S" << i << " " << ofToString(status.scale[i]);
//        cout << endl;
//    
//    }
    
//    // Update Length of Gesture
//    if ((gvf_handler->getGVF()->getState() != ofxGVF::STATE_CLEAR) && gvf_handler->getIsPlaying()) {
//        gesture_length->setTextString(ofToString(gvf_handler->getCurrentGesture()->getTemplateLength()));
//    }
    
}


//--------------------------------------------------------------
//MARK: GVF Settings
//--------------------------------------------------------------
void piafInterface::SetSettingsGUI() {
    
    settings_gui = new ofxUISuperCanvas("GVF Settings");
    
    // Parameters
    settings_gui->addNumberDialer("N Particles", 10, 10000, gvf_handler->getGVF()->getNumberOfParticles(), 0);
    settings_gui->addNumberDialer("Resampling Threshold", 100, 10000, gvf_handler->getGVF()->getResamplingThreshold(), 0);
    tolerance = settings_gui->addNumberDialer("Tolerance",  0.01, 2.0, gvf_handler->getGVF()->getTolerance(), 3);
    settings_gui->addNumberDialer("Distribution", 0.0, 2.0, gvf_handler->getGVF()->getDistribution(), 2);
    
    // !!!: Variance Coefficients (currently crash)
    settings_gui->addNumberDialer("Phase Variance", 0.000001, 0.1, gvf_handler->getGVF()->getPhaseVariance(), 5);
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
    
    // Update Tolerance
    tolerance->setValue(gvf_handler->getGVF()->getTolerance());
    
    // !!!: Temp: Feedback for changing settings
//    cout << "VARIANCES";
//    cout << " phase " << gvf_handler->getGVF()->getPhaseVariance();
//    cout << " speed " << gvf_handler->getGVF()->getSpeedVariance();
//    cout << " scale " << gvf_handler->getGVF()->getScaleVariance()[0];
//    cout << " rotation " << gvf_handler->getGVF()->getRotationVariance()[0];
//    cout << endl;

}


// --------------------------------
// MARK: Load/Save
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
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------




//
////--------------------------------------------------------------
//void gvfPianoInterface::update(){
//
//
//    // TODO below
//    /*
//     if (templates.size() != piano_follower_p->getTemplatesLearned())
//     updateTemplatesGui(piano_follower_p->getTemplatesLearned());
//
//     // Update templates after gesture has been learned only.
//     if (piano_follower_p->getState() == ofxGVF::STATE_FOLLOWING) {
//     updateTemplates(piano_follower_p->getTemplatesLearned());
//     }
//     */
//
//    templates_gui->draw();
//}
//


////--------------------------------------------------------------
////MARK: Gestures GUI
////--------------------------------------------------------------
//void gvfPianoInterface::initialiseGestureGui(vector<vector<float> >* gesture_buffer) {
//    
//    int graphWidth = 800;
//    int graphHeight = 120;
//    int maxPhraseLength = 410000;
//    
//    string graph_name = "Current Gesture Graph";
//    
//    gesture_gui = new ofxUICanvas(250, 0, graphWidth, graphHeight);
//    
//    gesture_gui->setColorBack(ofColor::grey);
//    gesture_gui->setDrawBack(true);
//    gesture_gui->setPadding(0);
//    
//    //FIXME: Sizing of the graph?
//    current_gesture_graph = new ofxUIMovingGraphX(graphWidth - 50, graphHeight - 50, gesture_buffer,
//                                                  maxPhraseLength, -1., 1., graph_name);
//    
//    
//    current_gesture_graph->setDrawBack(true);
//    current_gesture_graph->setColorBack(ofColor::green);
//    
//    gesture_gui->addWidgetDown(current_gesture_graph);
//    
//}
//
//void gvfPianoInterface::updateGestureGui() {
//    //current_gesture_graph->draw();
//}
//


////--------------------------------------------------------------
////MARK: Templates GUI
////--------------------------------------------------------------
//void gvfPianoInterface::initialiseTemplatesGui() {
//    
//    int graphWidth = 800;
//    int graphHeight = 800;
//    
//    templates_gui = new ofxUICanvas(250, 120, graphWidth, graphHeight);
//    
//    templates_gui->setColorBack(ofColor::paleTurquoise);
//    templates_gui->setDrawBack(true);
//    
//    // add a listener (for audio mostly)
//    // ofAddListener(templatesCanvas->newGUIEvent,this,&ofApp::templatesGuiEvent);
//    
//    
//}


////--------------------------------------------------------------
//void gvfPianoInterface::updateTemplatesGui(int nTemplates) {
//    
//    
//    // ???: Dyamic sizing or scrolling?
//    
//    int graphWidth = 800;
//    int graphHeight = 100;
//    int maxPhraseLength = 41000;
//    
//    templates.resize(nTemplates);
//    templates_gui->removeWidgets();
//    
//    for (int i = 0; i < nTemplates; ++i) {
//        
//        // TODO change access, create methods in the hanlder
//        
//        // ???: We copy the vector, perhaps can do without copying.
//        templates.at(i).data.resize(handler->getGVF()->getGestureTemplate(i).getTemplateLength(),
//                                    vector<float>(handler->getGVF()->getGestureTemplate(i).getNumberDimensions()));
//        templates.at(i).data = handler->getGVF()->getGestureTemplate(i).getTemplateRaw();
//        // Add Graph of data (static data)
//        templates.at(i).graph = new ofxUIMovingGraphX(graphWidth - 10, graphHeight,
//                                                      &templates.at(i).data, maxPhraseLength, -1., 1., "graph");
//        templates.at(i).graph->setDrawBack(true);
//        templates.at(i).graph->setColorBack(ofColor::green);
//        // Add Phase Slider (dynamic)
//        templates_gui->addWidgetDown(templates.at(i).graph);
//        
//        templates.at(i).phase_slider = new ofxUISlider("slider", 0., 1., &templates.at(i).phase, graphWidth - 10, 20);
//        templates.at(i).phase_slider->setLabelVisible(false);
//        templates.at(i).phase_slider->setColorFill(ofColor::grey);
//        templates_gui->addWidgetDown(templates.at(i).phase_slider);
//    }
//}
//
////--------------------------------------------------------------
//void gvfPianoInterface::updateTemplates(int nTemplates) {
//    for (int i = 0; i < nTemplates; ++i) {
//        // TODO
//        /*
//         templates.at(i).phase = piano_follower_p->getTemplateInfo(i).phase;
//         templates.at(i).is_most_probable = (i == piano_follower_p->getIndexMostProbable());
//         if (templates.at(i).is_most_probable)
//         templates.at(i).phase_slider->setColorFill(ofColor::white);
//         else
//         templates.at(i).phase_slider->setColorFill(ofColor::grey);
//         }*/
//    }
//}
//
////
////// TODO: After writing proper draw function for gesture, call here.
////void gvfhandler::drawTemplates(float scale, int indexMostProbable, recognitionInfo infoMostProbable) {
////    int nTemplates = templates.size();
////    int px, py, w, h;
////
////    for(int i = 0; i < nTemplates; i++)
////    {
////        px = 300;
////        py = i * (h + 20);
////
////        ofRectangle templateRect = ofRectangle(px, py, w, h);
////        gvfGesture g = getTemplateGesture(i);
////        g.setAppearance(g.getColor(), 1.5, 255, 50, 1);
////        g.drawBoundaries = false;
////        if (i == indexMostProbable)
////            g.draw(templateRect, scale, infoMostProbable.probability);
////        else
////            g.draw(templateRect, scale);
////    }
////}
//
//





////--------------------------------------------------------------
////--------------------------------------------------------------
//// MARK: Kinect
////--------------------------------------------------------------
////--------------------------------------------------------------
//
//
//// Live skeleton display
////--------------------------------------------------------------
//void gvfPianoInterface::UpdateSkeleton(SkeletonDataPoint new_point) {
//    
//    // Draw CENTER OF MASS
//    int x_pos = (int) ((float) (new_point.center_of_mass.x) * (float) kinect_width / (float) depth_width);
//    int y_pos = (int) ((float) (new_point.center_of_mass.y)* (float) kinect_height / (float) depth_height);
//    
//    ofNoFill();
//    ofSetColor(0, 255, 255);
//    ofCircle(x_pos, y_pos, 5);
//    ofFill();
//    
//    
//    for (int i = 0; i < N_JOINTS; ++i) {
//        
//        ofSetColor(255, 0, 255);
//        
//        x_pos = (int) ((float) (new_point.joints[i].x) * (float) kinect_width / (float) depth_width);
//        y_pos = (int) ((float) (new_point.joints[i].y) * (float) kinect_height / (float) depth_height);
//        ofCircle(x_pos, y_pos, 5);
//    }
//    
//    // TODO: Draw bounding box
//    
//}
