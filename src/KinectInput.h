//
//  KinectInput.h
//
//  Created by Alejandro Van Zandt-Escobar on 03/04/2014.
//
//

#ifndef __mvf__KinectInput__
#define __mvf__KinectInput__

#include <iostream>

#include "ofMain.h"

#include "NiTE.h"

#define MAX_USERS 1
#define MAX_DEPTH 10000
                  
// ------------------------------------------------------
// ------------------------------------------------------
// Skeleton Gesture Structure
// ------------------------------------------------------
// ------------------------------------------------------


typedef struct SkeletonDataPoint {
    
    // Constructors
    SkeletonDataPoint():
    joints(NITE_JOINT_COUNT),
    confidences(NITE_JOINT_COUNT),
    joint_orientations(NITE_JOINT_COUNT),
    orientation_confidences(NITE_JOINT_COUNT),
    center_of_mass(ofPoint(0, 0, 0)),
    bounding_box_min(ofPoint(0, 0, 0)),
    bounding_box_max(ofPoint(0, 0, 0))
    {
    }
    
    ~SkeletonDataPoint()
    {
        joints.clear();
        confidences.clear();
        joint_orientations.clear();
        orientation_confidences.clear();
    }
    
    // Skeleton data
    vector<ofPoint> joints;
    vector<float> confidences;
    vector<ofQuaternion> joint_orientations;
    vector<float> orientation_confidences;
    
    ofPoint center_of_mass;
    ofPoint bounding_box_min;
    ofPoint bounding_box_max;
    
    float time_stamp;
    
} SkeletonDataPoint;

// ------------------------------------------------------
// ------------------------------------------------------
// MARK: Kinect Input Class
// ------------------------------------------------------
// ------------------------------------------------------


class KinectInput
{
    
public:
    
	KinectInput();
	virtual ~KinectInput();
    
	virtual openni::Status setup();
    void update();
    
    bool get_is_running(); // False is Kinect fails setup. 
    
    SkeletonDataPoint get_data(int user_id = 0);
    nite::SkeletonState get_state(int user_id = 0);
    
    ofImage* GetImage();
    void UpdateImage();
    
    // Utility
    ofPoint convert_world_to_depth(ofPoint coordinates);
    
    
private:
    
    bool is_running;
    
    openni::Device device;
    nite::UserTracker* user_tracker;
    nite::UserTrackerFrameRef userTrackerFrame;
    
    nite::SkeletonState user_states[MAX_USERS];
    bool users_lost[MAX_USERS];
    bool users_visible[MAX_USERS];
    
    // Depth Image
    ofImage depth_image;
    unsigned char *grayPixels;
    void UpdateDepth(openni::VideoFrameRef depth_frame);
    float* CalculateHistogram(int histogramSize, const openni::VideoFrameRef& depthFrame);
    
    // Utility
    ofPoint NitePointToOF(NitePoint3f point);
    ofQuaternion NiteQuatToOF(NiteQuaternion quat);
};

#endif /* defined(__mvf__KinectInput__) */
