//
//  KinectInput.h
//
//  Created by Alejandro Van Zandt-Escobar on 03/04/2014.
//
//

#ifndef __mvf__KinectInput__
#define __mvf__KinectInput__

#include <iostream>

#include "NiTE.h"

#include "ofMain.h"

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
    
    SkeletonDataPoint(vector<ofPoint> _joints,
                      vector<float> _confidences,
                      vector<ofQuaternion> _joint_orientations,
                      vector<float> _orientation_confidences,
                      ofPoint _center_of_mass,
                      ofPoint _bounding_box_min,
                      ofPoint _bounding_box_max):
    joints(NITE_JOINT_COUNT),
    confidences(NITE_JOINT_COUNT),
    joint_orientations(NITE_JOINT_COUNT),
    orientation_confidences(NITE_JOINT_COUNT),
    center_of_mass(_center_of_mass),
    bounding_box_min(_bounding_box_min),
    bounding_box_max(_bounding_box_max)
    {
        for (int i = 0; i < NITE_JOINT_COUNT; ++i) {
            joints[i] = _joints[i];
            confidences[i] = _confidences[i];
            joint_orientations[i] = _joint_orientations[i];
            orientation_confidences[i] = _orientation_confidences[i];
        }
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
    
    float timing;
    
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
    
    SkeletonDataPoint get_data(int user_id = 0);
    const nite::UserData& get_user(int user_id = 0);
    SkeletonDataPoint get_depth_data(int user_id = 0); // For display
    
    ofImage* GetImage();
    void UpdateImage();
    
    nite::SkeletonState get_state(int user_id = 0);
    
    ofPoint convert_world_to_depth(ofPoint coordinates);
    
    bool get_is_running();
    
private:
    
    static KinectInput* ms_self;
    
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
    
};

#endif /* defined(__mvf__KinectInput__) */
