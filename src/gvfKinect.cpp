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
current_gesture(-1, "Current"),
is_playing(false),
state(ofxGVF::STATE_CLEAR),
current_template(0)
{
  
}

// TODO
//--------------------------------------------------------------
gvfKinect::~gvfKinect() {
}


//--------------------------------------------------------------
void gvfKinect::setup(){  
  
  // TODO: TESTING INTEGRATING KINECT
  kinect_input.setup();
  
  // Setup all the gvf handlers (set what data they extract)
  gvf_handlers.push_back(new gvfKinectHandler(0, "Center of Mass", CENTER_OF_MASS));
  
}

//--------------------------------------------------------------
void gvfKinect::exit() {
  
}

//--------------------------------------------------------------
void gvfKinect::update(){
  
  int user_id = 0;
  
  SkeletonDataPoint data_point = kinect_input.get_data();
  
  update(data_point);
  
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
      // Add data to current gesture
      current_gesture.add_data(data_point);
      
      // Send to GVFs
      gvf_input(data_point);
      
      cout << "Most probable is " << gvf_handlers[0]->getIndexMostProbable() << endl;
    }
    else if (state == ofxGVF::STATE_CLEAR) {
      // Do nothing
    }
  }
  
}

//--------------------------------------------------------------
void gvfKinect::draw() {
  
}

//--------------------------------------------------------------
//--------------------------------------------------------------

//--------------------------------------------------------------
int gvfKinect::addTemplate() {
  
  int template_id = skeleton_templates.size();
  
  SkeletonGesture new_template(template_id, "Template");
  
  skeleton_templates.push_back(new_template);
  
  return template_id; // Last index
  
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
    vector<SkeletonGesture> blank;
    skeleton_templates.swap(blank);
    cout << "N templates after clear: " << get_n_templates() << endl;
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
  if (!is_playing) {
    
    if (state == ofxGVF::STATE_LEARNING) {
      current_template = addTemplate();
      
      // Add template to each gvf
      for (std::vector<gvfKinectHandler*>::iterator gvfh = gvf_handlers.begin();
           gvfh != gvf_handlers.end();
           ++gvfh) {
        (*gvfh)->addTemplate();
      }
      
      is_playing = true;
      
    }
    else if (state == ofxGVF::STATE_FOLLOWING ) {
      current_gesture.clear(); // Reset current_gesture
      
      is_playing = true;
    }
    else if (state == ofxGVF::STATE_CLEAR) {
      // Do nothing.
    }
    
  }
  
}

//--------------------------------------------------------------
void gvfKinect::stop() {
  if (is_playing) {
    if (state == ofxGVF::STATE_LEARNING) {
      cout << "New template of length " << skeleton_templates[current_template].get_length() << endl;
      
      is_playing = false;
    }
    else if (state == ofxGVF::STATE_FOLLOWING ) {
      cout << "Followed gesture of length " << current_gesture.get_length() << endl;
      
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
int gvfKinect::get_n_templates() {
  return skeleton_templates.size();
}

//--------------------------------------------------------------




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
    // write skeleton gesture (pass file by reference)
    
    SkeletonGesture* gesture = &skeleton_templates[i];
    
    gesture_file.addTag("Gesture");
    gesture_file.pushTag("Gesture", i);
    
    // Metadata
    gesture_file.addValue("Id", gesture->get_id());
    gesture_file.addValue("Name", gesture->get_name());
    
    // Data
    for (int j = 0; j < gesture->get_length(); ++j) {
      gesture_file.addTag("DataPoint");
      gesture_file.pushTag("DataPoint", j);
      
      SkeletonDataPoint point = gesture->get_data_point(j);
      
      gesture_file.addTag("BBoxMin");
      gesture_file.pushTag("BBoxMin", 0);
      gesture_file.addValue("X", point.bounding_box_min.x);
      gesture_file.addValue("Y", point.bounding_box_min.y);
      gesture_file.addValue("Z", point.bounding_box_min.z);
      gesture_file.popTag();
      
      gesture_file.addTag("BBoxMax");
      gesture_file.pushTag("BBoxMax", 0);
      gesture_file.addValue("X", point.bounding_box_max.x);
      gesture_file.addValue("Y", point.bounding_box_max.y);
      gesture_file.addValue("Z", point.bounding_box_max.z);
      gesture_file.popTag();
      
      gesture_file.addTag("CenterMass");
      gesture_file.pushTag("CenterMass", 0);
      gesture_file.addValue("X", point.center_of_mass.x);
      gesture_file.addValue("Y", point.center_of_mass.y);
      gesture_file.addValue("Z", point.center_of_mass.z);
      gesture_file.popTag();
      
      for (int k = 0; k < N_JOINTS; ++k) {
        
        ofPoint joint = point.joints[k];
        
        gesture_file.addTag("Joints");
        gesture_file.pushTag("Joints", k);
        gesture_file.addValue("X", joint.x);
        gesture_file.addValue("Y", joint.y);
        gesture_file.addValue("Z", joint.z);
        gesture_file.popTag();
        
      }
      
      gesture_file.popTag(); // "Data Point"
    
    }
    
    gesture_file.popTag(); // Pop Tag "Gesture"
    
  }
  
  cout << "written to file" << endl;
  
  gesture_file.save(filename);
  
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

// Load Gestures from Given File
//--------------------------------------------------------------
void gvfKinect::loadGestures(string filename) {
  
  ofxXmlSettings gesture_file;
  if (!gesture_file.loadFile(filename)) {
    cout << "file not loaded" << endl;
    return;
  }
  
  clear();
  learn();
  
  int num_gestures = gesture_file.getNumTags("Gesture");
  
  cout << num_gestures << " gestures" << endl;
  if (num_gestures < 1)
    return;
  
  for (int i = 0; i < num_gestures; ++i) {
    gesture_file.pushTag("Gesture", i);
    
    play();
    
    gesture_file.getValue("Id", -1);
    gesture_file.getValue("Name", "name");
    
    int num_data_points = gesture_file.getNumTags("DataPoint");
    
    for (int j = 0; j < num_data_points; ++j) {
      
      SkeletonDataPoint new_point;
      
      gesture_file.pushTag("DataPoint", j);
      
      
      gesture_file.pushTag("BBoxMin", 0);
      new_point.bounding_box_min = ofPoint(gesture_file.getValue("X", 0.0),
                                           gesture_file.getValue("Y", 0.0),
                                           gesture_file.getValue("Z", 0.0));
      gesture_file.popTag();
      
      gesture_file.pushTag("BBoxMax", 0);
      new_point.bounding_box_max = ofPoint(gesture_file.getValue("X", 0.0),
                                           gesture_file.getValue("Y", 0.0),
                                           gesture_file.getValue("Z", 0.0));
      gesture_file.popTag();
      
      gesture_file.pushTag("CenterMass", 0);
      new_point.center_of_mass = ofPoint(gesture_file.getValue("X", 0.0),
                                           gesture_file.getValue("Y", 0.0),
                                           gesture_file.getValue("Z", 0.0));
      gesture_file.popTag();

      
      for (int k = 0; k < N_JOINTS; ++k) {
        
        gesture_file.pushTag("Joints", k);
        new_point.joints.push_back(ofPoint(gesture_file.getValue("X", 0.0),
                                gesture_file.getValue("Y", 0.0),
                                gesture_file.getValue("Z", 0.0)));
        gesture_file.popTag();
        
      }
      
      update(new_point);
      
      gesture_file.popTag(); // "Data Point"
    }
    
    stop();
    
    gesture_file.popTag(); // "Gesture"
  }
  
}

