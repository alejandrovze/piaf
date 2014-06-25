//
//  KinectInput.cpp
//  mvf
//
//  Created by Alejandro Van Zandt-Escobar on 03/04/2014.
//
//

#include "KinectInput.h"

KinectInput* KinectInput::ms_self = NULL; //???: Is this needed?


//--------------------------------------------------------------
KinectInput::KinectInput()
{
	ms_self = this; //???: Is this needed?
    
    is_running = false;
    
	user_tracker = new nite::UserTracker;
    
    for (int i = 0; i < MAX_USERS; ++i) {
        user_states[i] = nite::SKELETON_NONE;
        users_lost[i] = false;
        users_visible[i] = false;
    }
    
}

//--------------------------------------------------------------
KinectInput::~KinectInput()
{
    delete user_tracker;
	nite::NiTE::shutdown();
	openni::OpenNI::shutdown();
    
	ms_self = NULL; //???: Is this needed?
}

//--------------------------------------------------------------
openni::Status KinectInput::setup()
{
    
    // Initialize openNI
	openni::Status rc = openni::OpenNI::initialize();
	if (rc != openni::STATUS_OK)
	{
		printf("Failed to initialize OpenNI\n%s\n", openni::OpenNI::getExtendedError());
		return rc;
	}
    
    // Open Device (Kinect)
	const char* deviceUri = openni::ANY_DEVICE;
    
	rc = device.open(deviceUri);
    
	if (rc != openni::STATUS_OK)
	{
		printf("Failed to open device\n%s\n", openni::OpenNI::getExtendedError());
		return rc;
	}
    
    // Initialize NiTE
	nite::NiTE::initialize();
    
	if (user_tracker->create(&device) != nite::STATUS_OK)
	{
		return openni::STATUS_ERROR;
	}
    
    // If all else has succeeded.
    
    is_running = true;
    
    cout << "Device was opened successfully.\n";
    
    return openni::STATUS_OK;
    
}

//--------------------------------------------------------------
void KinectInput::update() {
    
    nite::Status rc = user_tracker->readFrame(&userTrackerFrame);
    
	if (rc != nite::STATUS_OK)
	{
		printf("Failed to read frame\n");
		return;
	}
    
    const nite::Array<nite::UserData>& users = userTrackerFrame.getUsers();
    
    for (int user_id = 0; user_id < users.getSize() && user_id < MAX_USERS; ++user_id) {
        
        const nite::UserData& user = users[user_id];
        
        // Print information about user state.
        {
            if (user.isNew())
            {
                fprintf(stderr, "User %d: New\n", user_id);
            }
            else if (users_visible[user_id] != user.isVisible()) {
                if ((users_visible[user_id] = user.isVisible())) {
                    fprintf(stderr, "User %d: Visible user\n", user_id);
                }
            }
            else if (users_lost[user_id] != user.isLost()) {
                if ((users_lost[user_id] = user.isLost())) {
                    fprintf(stderr, "User %d: Lost user\n", user_id);
                }
            }
            
            if (user_states[user_id] != user.getSkeleton().getState()) {
                switch(user_states[user_id] = user.getSkeleton().getState()) // Set in array
                {
                    case nite::SKELETON_NONE:
                        fprintf(stderr, "User %d: Stopped tracking.\n", user_id);
                        break;
                    case nite::SKELETON_CALIBRATING:
                        fprintf(stderr, "User %d: Calibrating.\n", user_id);
                        break;
                    case nite::SKELETON_TRACKED:
                        fprintf(stderr, "User %d: Tracking.\n", user_id);
                        break;
                    case nite::SKELETON_CALIBRATION_ERROR_NOT_IN_POSE:
                    case nite::SKELETON_CALIBRATION_ERROR_HANDS:
                    case nite::SKELETON_CALIBRATION_ERROR_LEGS:
                    case nite::SKELETON_CALIBRATION_ERROR_HEAD:
                    case nite::SKELETON_CALIBRATION_ERROR_TORSO:
                        fprintf(stderr, "User %d: Calibration failed.\n", user_id);
                        break;
                }
            }
        }
    }
}

//--------------------------------------------------------------
nite::SkeletonState KinectInput::get_state(int user_id) {
    
    return user_states[user_id];
    
}

//--------------------------------------------------------------
bool KinectInput::get_is_running() {
    return is_running;
}

//--------------------------------------------------------------
// MARK: Data Access
//--------------------------------------------------------------

//--------------------------------------------------------------
const nite::UserData& KinectInput::get_user(int user_id) {
    
	const nite::Array<nite::UserData>& users = userTrackerFrame.getUsers();
    
    if (user_id >= users.getSize() || user_id >= MAX_USERS) {
        // Error
        return;
    }
    
    return users[user_id];
    
}

//--------------------------------------------------------------
SkeletonDataPoint KinectInput::get_data(int user_id) {
    
    SkeletonDataPoint data_point;
    
    const nite::UserData& user = get_user(user_id);
    
    nite::UserTracker* pUserTracker;
    
    if (user.isNew())
    {
        user_tracker->startSkeletonTracking(user.getId());
    }
    else if (!user.isLost())
    {
        
        if (user.getSkeleton().getState() == nite::SKELETON_TRACKED)
        {
            
            // Center of Mass
            
            ofPoint com = ofPoint(user.getCenterOfMass().x, user.getCenterOfMass().y, user.getCenterOfMass().z);
            
            data_point.center_of_mass = com;
            
            // Joints
            
            for (nite::JointType joint = nite::JOINT_HEAD; joint < nite::JOINT_RIGHT_FOOT; joint++) {
                
                const nite::Point3f joint_pos = user.getSkeleton().getJoint(joint).getPosition();
                
                // Joint position relative to center of mass.
                data_point.joints.at(joint) = (ofPoint(joint_pos.x, joint_pos.y, joint_pos.z) - com);
                
                data_point.confidences.at(joint) = user.getSkeleton().getJoint(joint).getPositionConfidence();
                
                // Joint orientations.
                
                // Convert from NiteQuaternion to ofQuaternion
                NiteQuaternion orientation = user.getSkeleton().getJoint(joint).getOrientation();
                data_point.joint_orientations.at(joint) = ofQuaternion(orientation.x, orientation.y, orientation.z, orientation.w);
                data_point.orientation_confidences.at(joint) = user.getSkeleton().getJoint(joint).getOrientationConfidence();
                
            }
            
            data_point.bounding_box_min = ofPoint(user.getBoundingBox().min.x, user.getBoundingBox().min.y, user.getBoundingBox().min.z);
            
            data_point.bounding_box_max = ofPoint(user.getBoundingBox().max.x, user.getBoundingBox().max.y, user.getBoundingBox().max.z);
        }
    }
    
    return data_point;
    
}

// ------------------------------------------------------
SkeletonDataPoint KinectInput::get_depth_data(int user_id) {
    
    SkeletonDataPoint depth_data_point;
    
    const nite::UserData& user = get_user(user_id);
    
    nite::UserTracker* pUserTracker;
    
    if (user.isNew())
    {
        user_tracker->startSkeletonTracking(user.getId());
    }
    else if (!user.isLost())
    {
        
        if (user.getSkeleton().getState() == nite::SKELETON_TRACKED)
        {
            // Joints
            
            for (nite::JointType joint = nite::JOINT_HEAD; joint <= nite::JOINT_RIGHT_FOOT; joint++) {
                
                const nite::Point3f joint_pos = user.getSkeleton().getJoint(joint).getPosition();
                
                const float joint_conf = user.getSkeleton().getJoint(joint).getPositionConfidence();
                
                depth_data_point.joints.at(joint) = convert_world_to_depth(ofPoint(joint_pos.x, joint_pos.y, joint_pos.z));
                depth_data_point.confidences.at(joint) = joint_conf;
                
                // Joint orientations.
                
                // Convert from NiteQuaternion to ofQuaternion
                NiteQuaternion orientation = user.getSkeleton().getJoint(joint).getOrientation();
                depth_data_point.joint_orientations.at(joint) = ofQuaternion(orientation.x, orientation.y, orientation.z, orientation.w);
                depth_data_point.orientation_confidences.at(joint) = user.getSkeleton().getJoint(joint).getOrientationConfidence();
                
            }
            
            // Center of Mass
            
            depth_data_point.center_of_mass = convert_world_to_depth(ofPoint(user.getCenterOfMass().x, user.getCenterOfMass().y, user.getCenterOfMass().z));
            
            depth_data_point.bounding_box_min = convert_world_to_depth(ofPoint(user.getBoundingBox().min.x, user.getBoundingBox().min.y, user.getBoundingBox().min.z));
            
            depth_data_point.bounding_box_max = convert_world_to_depth(ofPoint(user.getBoundingBox().max.x, user.getBoundingBox().max.y, user.getBoundingBox().max.z));
        }
    }
    
    return depth_data_point;
}


//--------------------------------------------------------------
ofPoint KinectInput::convert_world_to_depth(ofPoint coordinates) {
    
    float x;
    float y;
    
    user_tracker->convertJointCoordinatesToDepth(coordinates.x,
                                                 coordinates.y,
                                                 coordinates.z,
                                                 &x, &y);
    
    return ofPoint(x,y);
    
}



//--------------------------------------------------------------
// MARK: Displaying Depth Frame
//--------------------------------------------------------------

//--------------------------------------------------------------
ofImage* KinectInput::GetImage() {
    
    return &depth_image;
}

//--------------------------------------------------------------
void KinectInput::UpdateImage() {
    
    if (get_is_running()) {
        
        UpdateDepth(userTrackerFrame.getDepthFrame());

    }
}

//--------------------------------------------------------------
void KinectInput::UpdateDepth(openni::VideoFrameRef depth_frame)
{
    
    if (depth_frame.isValid()) {
        
        int res_x = depth_frame.getVideoMode().getResolutionX();
        int res_y = depth_frame.getVideoMode().getResolutionY();
        int numPixels = res_x * res_y;
        
        float* depth_histogram = CalculateHistogram(MAX_DEPTH, depth_frame);
        
        // Allocate image (happens once)
        if (!depth_image.isAllocated()) {
            
            depth_image.allocate(res_x, res_y, OF_IMAGE_GRAYSCALE);
            
            grayPixels = new unsigned char[numPixels];
            memset(grayPixels, 0, numPixels * sizeof(unsigned char));
            
            cout << "Kinect Image has been allocated.\n";
        }
        
        
        const openni::DepthPixel* depthPixels = (const openni::DepthPixel*) depth_frame.getData();
        
        for (int i = 0; i < numPixels; i++, depthPixels++) {
            grayPixels[i] = depth_histogram[*depthPixels];
        }
        
        depth_image.setFromPixels(grayPixels, res_x, res_y, OF_IMAGE_GRAYSCALE);
        
        int rowSize = depth_frame.getStrideInBytes() / sizeof(openni::DepthPixel);
        
    }
    else {
        cout << "Kinect Image frame is invalid.\n";
    }
    
}

//--------------------------------------------------------------
float* KinectInput::CalculateHistogram(int histogramSize,
                                       const openni::VideoFrameRef& depthFrame)
{
	const openni::DepthPixel* pDepth = (const openni::DepthPixel*)depthFrame.getData();
	int width = depthFrame.getWidth();
	int height = depthFrame.getHeight();
    
    static float pHistogram[MAX_DEPTH];
    
	// Calculate the accumulative histogram
	memset(pHistogram, 0, histogramSize*sizeof(float));
	int restOfRow = depthFrame.getStrideInBytes() / sizeof(openni::DepthPixel) - width;
    
	unsigned int nNumberOfPoints = 0;
    
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x, ++pDepth)
		{
			if (*pDepth != 0)
			{
				pHistogram[*pDepth]++;
				nNumberOfPoints++;
			}
		}
		pDepth += restOfRow;
	}
    
	for (int nIndex = 1; nIndex < histogramSize; nIndex++)
	{
		pHistogram[nIndex] += pHistogram[nIndex-1];
	}
    
	if (nNumberOfPoints)
	{
		for (int nIndex = 1; nIndex < histogramSize; nIndex++)
		{
			pHistogram[nIndex] = (256 * (1.0f - (pHistogram[nIndex] / nNumberOfPoints)));
		}
	}
    
    return pHistogram;
}
