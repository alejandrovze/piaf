//
//  KinectInput.cpp
//  mvf
//
//  Created by Alejandro Van Zandt-Escobar on 03/04/2014.
//
//

#include "KinectInput.h"


//--------------------------------------------------------------
KinectInput::KinectInput()
{
    
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
                // Start tracking skeleton if new user has been found. 
                user_tracker->startSkeletonTracking(user.getId());
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
    
    assert(user_id < MAX_USERS);
    return user_states[user_id];
    
}

//--------------------------------------------------------------
bool KinectInput::get_is_running() {
    return is_running;
}

//--------------------------------------------------------------
// MARK: Data Access
//--------------------------------------------------------------


// Get Tracking Data in OF-friendly format (convert Nite Points to OF points)
//--------------------------------------------------------------
SkeletonDataPoint KinectInput::get_data(int user_id) {
    
    assert(user_id < MAX_USERS);
    
    SkeletonDataPoint data_point;
    
    // Get Data is User exists and is tracked.  
	const nite::Array<nite::UserData>& users = userTrackerFrame.getUsers();
    if (user_id < users.getSize()) {
        
        const nite::UserData& user = users[user_id];
        
        if (user.getSkeleton().getState() == nite::SKELETON_TRACKED)
        {
            
            // Center of Mass and Bounding Box
            data_point.center_of_mass = NitePointToOF(user.getCenterOfMass());
            data_point.bounding_box_min = NitePointToOF(user.getBoundingBox().min);
            data_point.bounding_box_max = NitePointToOF(user.getBoundingBox().max);
            
            // Joints
            for (nite::JointType i = nite::JOINT_HEAD; i < nite::JOINT_RIGHT_FOOT; i++) {
                
                nite::SkeletonJoint joint = user.getSkeleton().getJoint(i);
                
                data_point.joints.at(i) = NitePointToOF(joint.getPosition());
                data_point.confidences.at(i) = joint.getPositionConfidence();
                
                data_point.joint_orientations.at(i) = NiteQuatToOF(joint.getOrientation());
                data_point.orientation_confidences.at(i) = joint.getOrientationConfidence();
                
            }
        }
    }

    return data_point;
    
}


//--------------------------------------------------------------
// MARK: Utilities
//--------------------------------------------------------------

//--------------------------------------------------------------
ofPoint KinectInput::NitePointToOF(NitePoint3f point) {
    return ofPoint(point.x, point.y, point.z);
}

//--------------------------------------------------------------
ofQuaternion KinectInput::NiteQuatToOF(NiteQuaternion quat) {
    return ofQuaternion(quat.x, quat.y, quat.z, quat.w);
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

// Returns a reference to the Kinect Depth image for display
//--------------------------------------------------------------
ofImage* KinectInput::GetImage() {
    return &depth_image;
}

//--------------------------------------------------------------
void KinectInput::UpdateImage() {
    
    if (get_is_running())
        UpdateDepth(userTrackerFrame.getDepthFrame());
}

//--------------------------------------------------------------
void KinectInput::UpdateDepth(openni::VideoFrameRef depth_frame)
{
    
    if (depth_frame.isValid()) {
        
        int res_x = depth_frame.getVideoMode().getResolutionX();
        int res_y = depth_frame.getVideoMode().getResolutionY();
        int numPixels = res_x * res_y;
        
        float* depth_histogram = CalculateHistogram(MAX_DEPTH, depth_frame);
        
        // Allocate image
        if (!depth_image.isAllocated()) {
            
            depth_image.allocate(res_x, res_y, OF_IMAGE_GRAYSCALE);
            
            grayPixels = new unsigned char[numPixels];
            memset(grayPixels, 0, numPixels * sizeof(unsigned char));
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
//--------------------------------------------------------------
float* KinectInput::CalculateHistogram(int histogramSize, const openni::VideoFrameRef& depthFrame)
{
	const openni::DepthPixel* pDepth = (const openni::DepthPixel*)depthFrame.getData();
    
	int width = depthFrame.getWidth();
	int height = depthFrame.getHeight();
    
    static float pHistogram[MAX_DEPTH];
    
	// Calculate the accumulative histogram
	memset(pHistogram, 0, histogramSize * sizeof(float));
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
