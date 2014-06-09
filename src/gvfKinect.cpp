//
//  gvfKinect.cpp
//  mvf
//
//  Created by Alejandro Van Zandt-Escobar on 28/03/2014.
//
//

#include "gvfKinect.h"


//--------------------------------------------------------------
gvfKinect::gvfKinect():
is_playing(false),
is_live(false),
state(ofxGVF::STATE_CLEAR),
current_template(-1),
current_record(-1)
{
  
}

// TODO
//--------------------------------------------------------------
gvfKinect::~gvfKinect() {
  
}


//--------------------------------------------------------------
void gvfKinect::setup(){  
  
  kinect_input.setup();
  
  if (kinect_input.get_is_running())
    is_live = true; // Only set to live if correctly set up the kinect.
  
  // Setup all the gvf handlers (set what data they extract)
  
  // MARK: Setup Handlers
//  gvf_handlers.push_back(new gvfKinectHandler("Head", HEAD, 3));
//  gvf_handlers.push_back(new gvfKinectHandler("Right_Elbow", RIGHT_ELBOW, 1));
//  gvf_handlers.push_back(new gvfKinectHandler("Left_Elbow", LEFT_ELBOW, 1));
//  gvf_handlers.push_back(new gvfKinectHandler("Right_Knee", RIGHT_KNEE, 1));
//  gvf_handlers.push_back(new gvfKinectHandler("Left_Knee", LEFT_KNEE, 1));
//  gvf_handlers.push_back(new gvfKinectHandler("Right_Shoulder", RIGHT_SHOULDER, 1));
//  gvf_handlers.push_back(new gvfKinectHandler("Left_Shoulder", LEFT_SHOULDER, 1));
  gvf_handlers.push_back(new gvfKinectHandler("Right_Hand", RIGHT_HAND, 3));
  gvf_handlers.push_back(new gvfKinectHandler("Left_Hand", LEFT_HAND, 3));
  
  
//  gvf_handlers.push_back(new gvfKinectHandler(1, "Center of Mass", CENTER_OF_MASS, 3));
//  gvf_handlers.push_back(new gvfKinectHandler(6, "Breadth", BREADTH, 1));
  
}

//--------------------------------------------------------------
void gvfKinect::exit() {
  
}

//--------------------------------------------------------------
void gvfKinect::update(){
  
  int user_id = 0;
  
  if (is_live) {
    kinect_input.update();
    
    current_point = kinect_input.get_data();
    
    current_point.timing;
  }
  else {
    if (is_playing && state == ofxGVF::STATE_FOLLOWING) {
      if (current_record >= 0 && current_record < skeleton_gesture_records.size()) {
        
        current_point = skeleton_gesture_records[current_record].get_data_point(playback_position);
        
        // Move forward, check whether or not we have reached end of gesture.
        ++playback_position;
        if (playback_position >= skeleton_gesture_records[current_record].get_length())
          stop();
      }
    }
  }
  
  update(current_point);
  
}

//--------------------------------------------------------------
void gvfKinect::update(SkeletonDataPoint data_point){

  int user_id = 0;
  
  if (is_playing) {
    if (state == ofxGVF::STATE_LEARNING) {
      
      // Add data to template
      skeleton_templates[current_template].add_data(data_point);
      
      // Send to GVFs
      gvf_input(data_point);
      
    }
    else if (state == ofxGVF::STATE_FOLLOWING ) {
      
      // Add data to record
      if (is_live)
        skeleton_gesture_records[current_record].add_data(data_point);
      
      // Send to GVFs
      gvf_input(data_point);
      
      update_status();
      
    }
    else if (state == ofxGVF::STATE_CLEAR) {
      // Do nothing
    }
  }
}

//
//--------------------------------------------------------------
void gvfKinect::update_status() {
  
  // Update probabilities for each "meta-gesture"
  
  
  
  // Update variations for each "meta-gesture"
  
  
  
  // Set "most probable gesture". 
  
  
}


//--------------------------------------------------------------
//--------------------------------------------------------------

//--------------------------------------------------------------
int gvfKinect::add_template() {
  
  int template_id = skeleton_templates.size();
  
  SkeletonGesture new_template(template_id, "Template");
  
  skeleton_templates.push_back(new_template);
  
  return template_id; // Last index
  
}

// Adds a recorded gesture
int gvfKinect::add_record() {
  
  int record_id = skeleton_gesture_records.size();
  
  SkeletonGesture new_record(record_id, "Record");
  
  skeleton_gesture_records.push_back(new_record);
  
  return record_id; // Last index
  
}


// Set States:
//--------------------------------------------------------------
void gvfKinect::learn() {
  if (!is_playing) {
  
    state = ofxGVF::STATE_LEARNING;
    
    // Set state for each gvf
    for (std::vector<gvfKinectHandler*>::iterator gvfh = gvf_handlers.begin();
         gvfh != gvf_handlers.end();
         ++gvfh) {
      (*gvfh)->setState(state);
    }
    
  }
  else {
    // Do not change states while playing.
  }
}

//--------------------------------------------------------------
void gvfKinect::follow() {
  // Only change to following if not currently playing and there are recorded templates. 
  if (!is_playing && (get_n_templates() > 0)) {
    
    state = ofxGVF::STATE_FOLLOWING;
    
    // Set state for each gvf
    for (std::vector<gvfKinectHandler*>::iterator gvfh = gvf_handlers.begin();
         gvfh != gvf_handlers.end();
         ++gvfh) {
      (*gvfh)->setState(state);
    }
  }
  else {
    // Do not change states while playing.
  }
}

//--------------------------------------------------------------
void gvfKinect::clear() {
  if (!is_playing) {
    skeleton_templates.clear();
    cout << "N templates after clear: " << get_n_templates() << endl;
    
    skeleton_gesture_records.clear();
    cout << "N records after clear: " << get_n_records() << endl;
    
    state = ofxGVF::STATE_CLEAR;
    
    // Set state for each gvf
    for (std::vector<gvfKinectHandler*>::iterator gvfh = gvf_handlers.begin();
         gvfh != gvf_handlers.end();
         ++gvfh) {
      (*gvfh)->setState(state);
    }
  }
  else {
    // Do not change states while playing.
  }
}

//MARK: Sets
//--------------------------------------------------------------
void gvfKinect::play() {
  
  // Switch if stopped and in learning or following mode.
  if (!is_playing && state != ofxGVF::STATE_CLEAR) {
    
    // Switch State
    is_playing = true;
    
    if (state == ofxGVF::STATE_LEARNING) {
      current_template = add_template();
      
      // Add template to each gvf
      for (std::vector<gvfKinectHandler*>::iterator gvfh = gvf_handlers.begin();
           gvfh != gvf_handlers.end();
           ++gvfh) {
        (*gvfh)->addTemplate();
      }
      
    }
    else if (state == ofxGVF::STATE_FOLLOWING ) {
      
      if (is_live)
        current_record = add_record();
      else
        playback_position = 0;
      
    }
    
  }
  
}

//--------------------------------------------------------------
void gvfKinect::stop() {
  if (is_playing) {
    if (state == ofxGVF::STATE_LEARNING) {
      cout << "New template of length " << skeleton_templates[current_template].get_length() << endl;
      
      
      for (vector<gvfKinectHandler *>::iterator handler_it = gvf_handlers.begin();
           handler_it != gvf_handlers.end();
           ++handler_it) {
        
        cout << "Templates: " << (*handler_it)->getTemplateCount() << endl;
        
        //    int gvf_index = std::distance(gvf_handlers.begin(), handler_it);
        //
        //    cout << "Most probable is " << (*handler_it)->getIndexMostProbable() << " of "
        //    << (*handler_it)->getTemplateCount() << " templates" << endl;
      }

      
      is_playing = false;
    }
    else if (state == ofxGVF::STATE_FOLLOWING ) {
      
      if (is_live)
        cout << "Followed gesture of length " << skeleton_gesture_records[current_record].get_length() << endl;
      else
        cout << "Played back gesture of length " << skeleton_gesture_records[current_record].get_length() << endl;
      
      is_playing = false;
    }
    else if (state == ofxGVF::STATE_CLEAR) {
      // Do nothing
    }
  }
}

//--------------------------------------------------------------
void gvfKinect::gvf_input(SkeletonDataPoint data_point) {
  
  for (vector<gvfKinectHandler *>::iterator handler_it = gvf_handlers.begin();
       handler_it != gvf_handlers.end();
       ++handler_it) {
    
    (*handler_it)->gvf_data(data_point);
    
//    int gvf_index = std::distance(gvf_handlers.begin(), handler_it);
//    
//    cout << "Most probable is " << (*handler_it)->getIndexMostProbable() << " of "
//    << (*handler_it)->getTemplateCount() << " templates" << endl;
  }
  
}

//--------------------------------------------------------------
void gvfKinect::set_live(bool _is_live) {
  is_live = _is_live;
}

//--------------------------------------------------------------
void gvfKinect::set_current_record(int _current_record) {
  
  if (!is_playing) {
    if (_current_record >= 0 && _current_record < get_n_records())
      current_record = _current_record;
  }
}



//MARK: GETS
//--------------------------------------------------------------
ofxGVF::ofxGVFState gvfKinect::get_state() {
  return state;
}

//--------------------------------------------------------------
bool gvfKinect::get_is_playing() {
  return is_playing;
}

//--------------------------------------------------------------
bool gvfKinect::get_is_live() {
  return is_live;
}

//--------------------------------------------------------------
int gvfKinect::get_n_templates() {
  return skeleton_templates.size();
}

//--------------------------------------------------------------
int gvfKinect::get_n_records() {
  return skeleton_gesture_records.size();
}

//--------------------------------------------------------------
int gvfKinect::get_current_record() {
  return current_record;
}

//--------------------------------------------------------------
openni::VideoFrameRef gvfKinect:: get_depth_frame() {
  if (is_live)
    return kinect_input.get_depth_frame();
  else
    return;
}

//--------------------------------------------------------------
SkeletonDataPoint gvfKinect::get_current_point() {
  return current_point;
}

//--------------------------------------------------------------
SkeletonDataPoint gvfKinect::get_depth_data_point() {
  return kinect_input.get_depth_data();
}

//--------------------------------------------------------------
string gvfKinect::get_tracking_state() {
  
  switch(kinect_input.get_state())
  {
    case nite::SKELETON_NONE:
      return "NOT TRACKING";
      break;
    case nite::SKELETON_CALIBRATING:
      return "CALIBRATING";
      break;
    case nite::SKELETON_TRACKED:
      return "TRACKING";
      break;
    case nite::SKELETON_CALIBRATION_ERROR_NOT_IN_POSE:
    case nite::SKELETON_CALIBRATION_ERROR_HANDS:
    case nite::SKELETON_CALIBRATION_ERROR_LEGS:
    case nite::SKELETON_CALIBRATION_ERROR_HEAD:
    case nite::SKELETON_CALIBRATION_ERROR_TORSO:
      return "ERROR";
      break;
  }
}

//--------------------------------------------------------------
int gvfKinect::get_playback_position() {
  return playback_position;
}

//--------------------------------------------------------------
int gvfKinect::get_current_record_length() {
  if (skeleton_gesture_records.size() > current_record)
    return skeleton_gesture_records[current_record].get_length();
  else
    return 0;
}

int gvfKinect::get_n_gvfs() {
  return gvf_handlers.size();
}


//--------------------------------------------------------------
RecognitionData gvfKinect::get_recognition_data() {
  
  RecognitionData recog_data;
  
  recog_data.n_gvf = gvf_handlers.size(); // Number of GVFs
  
  for (std::vector<gvfKinectHandler*>::iterator gvfh = gvf_handlers.begin();
       gvfh != gvf_handlers.end();
       ++gvfh) {
    
    recog_data.gvf_names.push_back((*gvfh)->gvf_name);
    
    recog_data.most_probable_gesture.push_back((*gvfh)->getIndexMostProbable());
  
    recog_data.recognition_info.push_back((*gvfh)->getRecogInfoOfMostProbable());
    
  }
  
  return recog_data;
  
}



//--------------------------------------------------------------
// TODO: weighting to get "most probable gesture"
int gvfKinect::get_most_probable() {
  return -1;
}

int gvfKinect::get_most_probable(int gvf_index) {
  
  if (gvf_index < gvf_handlers.size()) {
    return gvf_handlers[gvf_index]->getIndexMostProbable();
  }
  
  else {
    cout << "PROBLEM" << endl;
    return -1;
  }
}

string gvfKinect::get_gvf_name(int gvf_index) {
  if (gvf_index < gvf_handlers.size()) {
    return gvf_handlers[gvf_index]->gvf_name;
  }
  return "NOT ACTIVE";
}


//--------------------------------------------------------------
SkeletonGesture* gvfKinect::get_template(int template_index) {
  
  if (template_index >= -1 && template_index < skeleton_templates.size()) {
    return &skeleton_templates[template_index];
  }
  else
    return NULL;
  
}

//--------------------------------------------------------------
// MARK: Load / Save
//--------------------------------------------------------------

// Opens Dialog to Save
//--------------------------------------------------------------
void gvfKinect::saveGestures() {
  
  ofFileDialogResult dialogResult = ofSystemSaveDialog("my_gestures.xml", "Save gestures");
  
  if(!dialogResult.bSuccess)
    return;
  
  cout << "Saving to " << dialogResult.filePath << endl;
  
  saveGestures(dialogResult.filePath);
}

//--------------------------------------------------------------
void gvfKinect::saveGestures(string filename) {
  
  ofxXmlSettings gesture_file;
  
  for (int i = 0; i < skeleton_templates.size(); ++i) {
    
    SkeletonGesture* saved_gesture = &skeleton_templates[i];
    
    writeGesture(&gesture_file, saved_gesture, i, "Template");
    
    cout << "Wrote template of length " << saved_gesture->get_length() << endl;
    
  }
  
  for (int i = 0; i < skeleton_gesture_records.size(); ++i) {
    
    SkeletonGesture* saved_gesture = &skeleton_gesture_records[i];
    
    writeGesture(&gesture_file, saved_gesture, i, "Record");
    
    cout << "Wrote record of length " << saved_gesture->get_length() << endl;
    
  }
  
  gesture_file.save(filename);
  
}

void gvfKinect::writeGesture(ofxXmlSettings* gesture_file, SkeletonGesture* saved_gesture, int id, string type) {
  
  gesture_file->addTag(type); // Template or Record
  gesture_file->pushTag(type, id);
  
  // Metadata
  gesture_file->addValue("Id", saved_gesture->get_id());
  gesture_file->addValue("Name", saved_gesture->get_name());
  
  // Data
  for (int j = 0; j < saved_gesture->get_length(); ++j) {
    gesture_file->addTag("DataPoint");
    gesture_file->pushTag("DataPoint", j);
    
    SkeletonDataPoint point = saved_gesture->get_data_point(j);
    
    gesture_file->addTag("BBoxMin");
    gesture_file->pushTag("BBoxMin", 0);
    gesture_file->addValue("X", point.bounding_box_min.x);
    gesture_file->addValue("Y", point.bounding_box_min.y);
    gesture_file->addValue("Z", point.bounding_box_min.z);
    gesture_file->popTag();
    
    gesture_file->addTag("BBoxMax");
    gesture_file->pushTag("BBoxMax", 0);
    gesture_file->addValue("X", point.bounding_box_max.x);
    gesture_file->addValue("Y", point.bounding_box_max.y);
    gesture_file->addValue("Z", point.bounding_box_max.z);
    gesture_file->popTag();
    
    gesture_file->addTag("CenterMass");
    gesture_file->pushTag("CenterMass", 0);
    gesture_file->addValue("X", point.center_of_mass.x);
    gesture_file->addValue("Y", point.center_of_mass.y);
    gesture_file->addValue("Z", point.center_of_mass.z);
    gesture_file->popTag();
    
    for (int k = 0; k < N_JOINTS; ++k) {
      
      ofPoint joint = point.joints[k];
      
      gesture_file->addTag("Joints"); // Position
      gesture_file->pushTag("Joints", k);
      gesture_file->addValue("X", joint.x);
      gesture_file->addValue("Y", joint.y);
      gesture_file->addValue("Z", joint.z);
      gesture_file->addValue("Confidence", point.confidences[k]); // !!!: Added later.
      gesture_file->popTag(); // Pop "Joints"
      
      
      ofQuaternion orientation = point.joint_orientations[k];
      
      gesture_file->addTag("Orientation");
      gesture_file->pushTag("Orientation", k);
      gesture_file->addValue("X", orientation.x());
      gesture_file->addValue("Y", orientation.y());
      gesture_file->addValue("Z", orientation.z());
      gesture_file->addValue("W", orientation.w());
      gesture_file->addValue("Confidence", point.orientation_confidences[k]); // !!!: Added later.
      gesture_file->popTag(); // Pop "Orientation". 
      
    }
    
    gesture_file->popTag(); // "Data Point"
    
  }
  
  gesture_file->popTag(); // Pop Tag Gesture-Type (Template or Record)
  
}

// Opens Dialog to Load gestures
//--------------------------------------------------------------
void gvfKinect::loadGestures() {
  
  ofFileDialogResult dialogResult = ofSystemLoadDialog("Select the xml file containing gesture data");
  
  if(!dialogResult.bSuccess)
    return;
  
  cout << "Loading file " << dialogResult.filePath << endl;
  
  
  loadGestures(dialogResult.filePath);
}

//--------------------------------------------------------------
void gvfKinect::loadCsv() {
  
  ofFileDialogResult dialogResult = ofSystemLoadDialog("Select the csv file containing gesture data");
  
  if(!dialogResult.bSuccess)
    return;
  
  string filename = dialogResult.filePath;
  
  wng::ofxCsv data_file;
  
  data_file.loadFile(filename, " ");
  
  
  cout << "Num rows of new file is " << data_file.numRows << endl;
  cout << "Num columns of new file is " << data_file.numCols << endl;
  
  int nui_joint_pos[N_JOINTS];
  
  nui_joint_pos[JOINT_HEAD] = NUI_SKELETON_POSITION_HEAD;
  nui_joint_pos[JOINT_NECK] = NUI_SKELETON_POSITION_SHOULDER_CENTER;
  nui_joint_pos[JOINT_LEFT_SHOULDER] = NUI_SKELETON_POSITION_SHOULDER_LEFT;
  nui_joint_pos[JOINT_RIGHT_SHOULDER] = NUI_SKELETON_POSITION_SHOULDER_RIGHT;
  nui_joint_pos[JOINT_LEFT_ELBOW] =  NUI_SKELETON_POSITION_ELBOW_LEFT;
  nui_joint_pos[JOINT_RIGHT_ELBOW] = NUI_SKELETON_POSITION_ELBOW_RIGHT;
  nui_joint_pos[JOINT_LEFT_HAND] = NUI_SKELETON_POSITION_HAND_LEFT;
  nui_joint_pos[JOINT_RIGHT_HAND] = NUI_SKELETON_POSITION_HAND_RIGHT;
  nui_joint_pos[JOINT_TORSO] = NUI_SKELETON_POSITION_SPINE;
  nui_joint_pos[JOINT_LEFT_HIP] =NUI_SKELETON_POSITION_HIP_LEFT;
  nui_joint_pos[JOINT_RIGHT_HIP] = NUI_SKELETON_POSITION_HIP_RIGHT;
  nui_joint_pos[JOINT_LEFT_KNEE] = NUI_SKELETON_POSITION_KNEE_LEFT;
  nui_joint_pos[JOINT_RIGHT_KNEE] = NUI_SKELETON_POSITION_KNEE_RIGHT;
  nui_joint_pos[JOINT_LEFT_FOOT] = NUI_SKELETON_POSITION_FOOT_LEFT;
  nui_joint_pos[JOINT_RIGHT_FOOT] = NUI_SKELETON_POSITION_FOOT_RIGHT;
  
//  NUI_SKELETON_POSITION_HIP_CENTER = 0,
//  NUI_SKELETON_POSITION_WRIST_RIGHT = 10,
//  NUI_SKELETON_POSITION_ANKLE_LEFT = 14,
//  NUI_SKELETON_POSITION_FOOT_LEFT = 15,
//  NUI_SKELETON_POSITION_ANKLE_RIGHT = 18,
  
  
  clear();
  
  learn();
  
  play();
  
  
  for (int i = 0; i < data_file.numRows; ++i) {
    
    SkeletonDataPoint new_point;
    
    // No bounding box.
    
    // No center of mass.
    
    // Get joints
    
    for (int k = 0; k < N_JOINTS; ++k) {
      
      int joint_index = 4 * nui_joint_pos[k] + 1;
      
      new_point.joints[k] = ofPoint(atof(data_file.data[i][joint_index].c_str()),
                                    atof(data_file.data[i][joint_index + 1].c_str()),
                                    atof(data_file.data[i][joint_index + 2].c_str())) * (- 1000.);
      
      new_point.confidences[k] = 1.0;
      
      
    }
    
    
    // No orientations
    
    update(new_point);
  }
  
  stop();
  
  
  cout << "Loaded CSV template of length " << data_file.numRows << endl;
  
//    
//    for (int k = 0; k < N_JOINTS; ++k) {
//      
//      gesture_file->pushTag("Joints", k);
//      new_point.joints[k] = ofPoint(gesture_file->getValue("X", 0.0),
//                                    gesture_file->getValue("Y", 0.0),
//                                    gesture_file->getValue("Z", 0.0));
//      new_point.confidences[k] = gesture_file->getValue("Confidence", 1.0); // Default confidence 1.0 (if not recorded).
//      gesture_file->popTag();
//      
//      }
//    }


}

// Load Gestures from Given File
//--------------------------------------------------------------
void gvfKinect::loadGestures(string filename) {
  
  ofxXmlSettings gesture_file;
  if (!gesture_file.loadFile(filename)) {
    cout << "file not loaded" << endl;
    return;
  }
  
  clear();
  
  // Load recorded gestures (for playback)
  int num_records = gesture_file.getNumTags("Record");
  cout << num_records << " gesture records" << endl;
  
  for (int i = 0; i < num_records; ++i) {
    loadGesture(&gesture_file, i, "Record");
  }
  
  // Learn templates
  learn();
  
  int num_templates = gesture_file.getNumTags("Template");
  cout << num_templates << " templates" << endl;
  
  for (int i = 0; i < num_templates; ++i) {
    loadGesture(&gesture_file, i, "Template");
  }
  
}

//--------------------------------------------------------------
void gvfKinect::loadGesture(ofxXmlSettings* gesture_file, int id, string type) {
  
  gesture_file->pushTag(type, id);
  
  gesture_file->getValue("Id", -1);
  
  string name = gesture_file->getValue("Name", "name");
  
  bool is_template = (type.compare("Template") == 0);
  
  if (is_template)
    play();
  else
    current_record = add_record();
  
  int num_data_points = gesture_file->getNumTags("DataPoint");
  
  for (int j = 0; j < num_data_points; ++j) {
    
    SkeletonDataPoint new_point;
    
    gesture_file->pushTag("DataPoint", j);
    
    
    gesture_file->pushTag("BBoxMin", 0);
    new_point.bounding_box_min = ofPoint(gesture_file->getValue("X", 0.0),
                                         gesture_file->getValue("Y", 0.0),
                                         gesture_file->getValue("Z", 0.0));
    gesture_file->popTag();
    
    gesture_file->pushTag("BBoxMax", 0);
    new_point.bounding_box_max = ofPoint(gesture_file->getValue("X", 0.0),
                                         gesture_file->getValue("Y", 0.0),
                                         gesture_file->getValue("Z", 0.0));
    gesture_file->popTag();
    
    gesture_file->pushTag("CenterMass", 0);
    new_point.center_of_mass = ofPoint(gesture_file->getValue("X", 0.0),
                                       gesture_file->getValue("Y", 0.0),
                                       gesture_file->getValue("Z", 0.0));
    gesture_file->popTag();
    
    
    for (int k = 0; k < N_JOINTS; ++k) {
      
      gesture_file->pushTag("Joints", k);
      new_point.joints[k] = ofPoint(gesture_file->getValue("X", 0.0),
                                         gesture_file->getValue("Y", 0.0),
                                         gesture_file->getValue("Z", 0.0));
      new_point.confidences[k] = gesture_file->getValue("Confidence", 1.0); // Default confidence 1.0 (if not recorded). 
      gesture_file->popTag();
      
      // Confidence
      if (gesture_file->tagExists("Orientation", k)) {
        gesture_file->pushTag("Orientation", k);
        new_point.joint_orientations[k] = ofQuaternion(gesture_file->getValue("X", 0.0),
                                      gesture_file->getValue("Y", 0.0),
                                      gesture_file->getValue("Z", 0.0),
                                      gesture_file->getValue("W", 0.0));
        new_point.orientation_confidences[k] = gesture_file->getValue("Confidence", 0.0); // Default confidence 0.0 (might not have orientation in older records).
        gesture_file->popTag();
      }
    }
    
    if (is_template)
      update(new_point);
    else
      skeleton_gesture_records[current_record].add_data(new_point);
    
    gesture_file->popTag(); // "Data Point"
  }
  
  if (is_template)
    stop();
  
  gesture_file->popTag(); // "Gesture"
  
  // Confirmation
  cout << type << " added of length " << num_data_points << endl;
  
}

// TODO
//--------------------------------------------------------------
void gvfKinect::delete_template(int template_index) {
  
  if (!is_playing) {
    if (template_index >= -1 && template_index < skeleton_templates.size()) {
      // remove from template gestures
      // 
      
      return &skeleton_templates[template_index];
    }
    else {
      // No action
    }
  }
  
}
