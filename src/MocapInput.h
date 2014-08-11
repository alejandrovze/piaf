//
//  MocapInput.h
//  piaf
//
//  Created by Alejandro Van Zandt-Escobar on 25/07/2014.
//
//

#ifndef __piaf__MocapInput__
#define __piaf__MocapInput__

#include <iostream>

#include "ofxOsc.h"

#define MOCAP_PORT 8000

typedef struct {
    
    string name;
    
    ofVec3f position;
    ofQuaternion orientation;
    
} RigidBody;


class MocapInput {
    
public:
    
    void setup(int port = MOCAP_PORT);
    void update();
    
    RigidBody GetRigidBody(int body_id);
    int get_n_body_parts();
    
    const vector<ofVec3f>& get_bounds();
    void ResetBounds();
    
private:
    
    // Struct Skeleton
    
    ofxOscReceiver osc_receiver;
    vector<RigidBody> body_parts;
    int n_body_parts;
    vector<ofVec3f> bounds;
    
    ofVec3f GetRigidBodyVector(ofxOscMessage& mocap_msg);
    ofQuaternion GetRigidBodyQuat(ofxOscMessage& mocap_msg);
    
    ofVec3f MinVec(ofVec3f vec1, ofVec3f vec2);
    ofVec3f MaxVec(ofVec3f vec1, ofVec3f vec2);
    
    bool bounds_initialised;
    
};


#endif /* defined(__piaf__MocapInput__) */
