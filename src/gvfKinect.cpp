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
state(ofxGVF::STATE_CLEAR)
{
  
}

//--------------------------------------------------------------
void gvfKinect::setup(){
  
  skeleton_input.setup();
  
}

//--------------------------------------------------------------
void gvfKinect::exit() {
  
}

//--------------------------------------------------------------
void gvfKinect::update(){
  
  skeleton_input.update();
  int user_id = 0;
  
  if (is_playing) {
    if (state == ofxGVF::STATE_LEARNING) {
      // Add data to template
      skeleton_templates[current_template].add_data(skeleton_input.get_skeleton(user_id),
                                                    skeleton_input.get_center_of_mass(user_id),
                                                    skeleton_input.get_bounding_box_min(user_id),
                                                    skeleton_input.get_bounding_box_max(user_id));

      
      // Send to GVFs
      
    }
    else if (state == ofxGVF::STATE_FOLLOWING ) {
      // Add data to current gesture
      current_gesture.add_data(skeleton_input.get_skeleton(user_id),
                               skeleton_input.get_center_of_mass(user_id),
                               skeleton_input.get_bounding_box_min(user_id),
                               skeleton_input.get_bounding_box_max(user_id));
      
      // Send to GVFs
      
    }
    else if (state == ofxGVF::STATE_CLEAR) {
      // Do nothing
    }
  }
  
}



//--------------------------------------------------------------
//--------------------------------------------------------------

//--------------------------------------------------------------
int gvfKinect::addTemplate() {
  
  SkeletonGesture new_template(skeleton_templates.size() - 1, "Template");
  
  skeleton_templates.push_back(new_template);
  
  return new_template.id; // Last index
  
}


// Set States:
//--------------------------------------------------------------
void gvfKinect::learn() {
  if (!is_playing) {
  
    state = ofxGVF::STATE_LEARNING;
  }
  else {
    // Do not change states while playing.
  }
}

//--------------------------------------------------------------
void gvfKinect::follow() {
  if (!is_playing) {
    
    state = ofxGVF::STATE_FOLLOWING;
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
  }
  else {
    // Do not change states while playing.
  }
}

//MARK: Sets
// TODO
//--------------------------------------------------------------
void gvfKinect::play() {
  if (!is_playing) {
    
    if (state == ofxGVF::STATE_LEARNING) {
      current_template = addTemplate();
      
      is_playing = true;
      
    }
    else if (state == ofxGVF::STATE_FOLLOWING ) {
      current_gesture.data.clear(); // Reset current_gesture
      
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
      
      is_playing = false;
    }
    else if (state == ofxGVF::STATE_CLEAR) {
      // Do nothing
    }
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

// MARK: Load / Save

// TODO
//--------------------------------------------------------------
void gvfKinect::saveGestures(string filename) {
  //    gvfh->saveGestures(filename);
}

// TODO
//--------------------------------------------------------------
void gvfKinect::loadGestures(string filename) {
  //    gvfh->loadGestures(filename);
  //    gvfh->gvf_follow();
}

