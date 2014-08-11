//
//  MocapInput.cpp
//  piaf
//
//  Created by Alejandro Van Zandt-Escobar on 25/07/2014.
//
//

#include "MocapInput.h"

// ------------------------------------------------------
void MocapInput::setup(int port) {
    
    osc_receiver.setup(port);
    
    n_body_parts = 2;
    
    body_parts = vector<RigidBody>(n_body_parts);
    
    // TODO: dynamically update
    body_parts[0].name = "hand";
    body_parts[1].name = "head";
    
    bounds = vector<ofVec3f>(2 * n_body_parts);
    
    bounds_initialised = false;
    
}

// ------------------------------------------------------
void MocapInput::update() {
    
    while (osc_receiver.hasWaitingMessages()) {
        
        ofxOscMessage mocap_msg;
        osc_receiver.getNextMessage(&mocap_msg);
        
        if (mocap_msg.getAddress() == "/body") {
            
            string name = mocap_msg.getArgAsString(0);
            int id;
            
            if (name == "hand")
                id = 0;
            else if (name == "head")
                id = 1;
            
            
            if (id < body_parts.size()) {
                
                body_parts[id].position = GetRigidBodyVector(mocap_msg);
                
                body_parts[id].orientation = GetRigidBodyQuat(mocap_msg);
                
                // Bounds
                if (bounds_initialised) {
                    bounds[id * 2] = MinVec(bounds[id * 2], body_parts[id].position);
                    bounds[id * 2 + 1] = MaxVec(bounds[id * 2 + 1], body_parts[id].position);
                }
                else {
                    bounds[id * 2] = body_parts[id].position;
                    bounds[id * 2 + 1] = body_parts[id].position;
                    bounds_initialised = true;
                }
            }
            else {
                // Deal with this better.
                cout << "Too many body parts." << endl;
            }
            
        }
        
    }
    
}

// ------------------------------------------------------
void MocapInput::ResetBounds() {
    bounds_initialised = false;
}

// ------------------------------------------------------
RigidBody MocapInput::GetRigidBody(int body_id) {
    
    return body_parts[body_id];
    
}

// ------------------------------------------------------
int MocapInput::get_n_body_parts() {
    return n_body_parts;
}

// ------------------------------------------------------
const vector<ofVec3f>& MocapInput::get_bounds() {
    
    // In case there is poor calibration, to prevent getting nan
    // values later
    
    for (int i = 0; i < n_body_parts; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (bounds[2 * i + 0][j] == bounds[2 * i + 1][j]) {
                bounds[2 * i + 0][j] = 0.;
                bounds[2 * i + 1][j] = 100.;
            }
        }
    }
    
    return bounds;
}


// ------------------------------------------------------
ofVec3f MocapInput::GetRigidBodyVector(ofxOscMessage& mocap_msg) {
    
    float x = mocap_msg.getArgAsFloat(1);
    float y = mocap_msg.getArgAsFloat(2);
    float z = mocap_msg.getArgAsFloat(3);
    
    return ofVec3f(x, y, z);
    
}

// ------------------------------------------------------
ofQuaternion MocapInput::GetRigidBodyQuat(ofxOscMessage& mocap_msg) {
    
    float x = mocap_msg.getArgAsFloat(4);
    float y = mocap_msg.getArgAsFloat(5);
    float z = mocap_msg.getArgAsFloat(6);
    float w = mocap_msg.getArgAsFloat(7);
    
    return ofQuaternion(x, y, z, w);
}

// ------------------------------------------------------
ofVec3f MocapInput::MinVec(ofVec3f vec1, ofVec3f vec2) {
    
    ofVec3f min_vec;
    
    for (int i = 0; i < 3; ++i)
        min_vec[i]= min(vec1[i], vec2[i]);
    
    return min_vec;
    
}

// ------------------------------------------------------
ofVec3f MocapInput::MaxVec(ofVec3f vec1, ofVec3f vec2) {
    
    ofVec3f max_vec;
    
    for (int i = 0; i < 3; ++i)
        max_vec[i]= max(vec1[i], vec2[i]);
    
    return max_vec;
    
}