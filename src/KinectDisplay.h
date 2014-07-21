//
//  KinectDisplay.h
//  piaf
//
//  Created by Alejandro Van Zandt-Escobar on 10/07/2014.
//
//

#ifndef __piaf__KinectDisplay__
#define __piaf__KinectDisplay__

#include <iostream>

#include "KinectInput.h"
#include "GVFHandler.h"


//--------------------------------------------------------------
// MARK: Kinect Constants / Enums
//--------------------------------------------------------------


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

static  const int NITE_JOINT_CONNECTIONS_COUNT = 14;

static const NiteJointType NiteJointConnections[NITE_JOINT_CONNECTIONS_COUNT][2] = {
    {NITE_JOINT_HEAD, NITE_JOINT_NECK},
    
    {NITE_JOINT_NECK, NITE_JOINT_TORSO},
    {NITE_JOINT_NECK, NITE_JOINT_LEFT_SHOULDER},
    {NITE_JOINT_NECK, NITE_JOINT_RIGHT_SHOULDER},
    
    {NITE_JOINT_LEFT_SHOULDER, NITE_JOINT_LEFT_ELBOW},
    {NITE_JOINT_LEFT_ELBOW, NITE_JOINT_LEFT_HAND},
    
    {NITE_JOINT_RIGHT_SHOULDER, NITE_JOINT_RIGHT_ELBOW},
    {NITE_JOINT_RIGHT_ELBOW, NITE_JOINT_RIGHT_HAND},
    
    {NITE_JOINT_TORSO, NITE_JOINT_LEFT_HIP},
    {NITE_JOINT_TORSO, NITE_JOINT_RIGHT_HIP},
    
    {NITE_JOINT_LEFT_HIP, NITE_JOINT_LEFT_KNEE},
    {NITE_JOINT_LEFT_KNEE, NITE_JOINT_LEFT_FOOT},
    
    {NITE_JOINT_RIGHT_HIP, NITE_JOINT_RIGHT_KNEE},
    {NITE_JOINT_RIGHT_KNEE, NITE_JOINT_RIGHT_FOOT}
};

class KinectDisplay {
    
public:
    
    void setup(KinectInput* input, GVFHandler* handler);
    
    void draw();
    
    void DrawKinect(SkeletonDataPoint skeleton, vector<int> joints_on = vector<int>(0));
    
private:
    
    KinectInput* kinect_input;
    GVFHandler* gvf_handler;
    ofImage* kinect_image;
    ofPoint ScaleToKinect(ofPoint init_point);
    ofMesh skeleton_mesh;
    void SetupSkeleton();
    
//    void DrawTemplate(ofxGVFGesture& gesture_template);
    
    void DrawTemplates();
    void DrawGesture();
    void DrawParticles();
    
    // COLORS
    void initColors();
    ofColor generateRandomColor();
    std::vector<ofColor> colors;
    
    void drawInteractionArea();
    ofEasyCam cam; // add mouse controls for camera movement
    
};


#endif /* defined(__piaf__KinectDisplay__) */
