//
//  gvfPianoInputs.cpp
//  oFxGVFxPiano
//
//  Created by Alejandro Van Zandt-Escobar on 26/11/2013.
//
//

#include "gvfPianoInputs.h"

//--------------------------------------------------------------
void gvfPianoInputs::setup(){
    
    kinect_input.setup();
    
    if (kinect_input.get_is_running())
        kinect_is_live = true; // Only set to live if correctly set up the kinect.
    
    accAddresses.push_back("/wax/14");
    
    int port = 8200;
    accReceiver.setup(port);
    
    // Initialize data input vector
    accData = std::vector<vector<float> >(2, vector<float>(3,0.0));
    
    // TODO: Write method for bounds settings.
    vector<float> aBounds;
    aBounds.push_back(-1.);
    aBounds.push_back(1.);
    accelerometerBounds = vector<vector<float> >(3, aBounds);
    
    // Default is track right hand, one accelerometer.
    // lacc racc lefth righth
    setInputs(true, false, false, false);
//    setInputs(false, false, false, true);
    
    is_writing = false;
    
}

// Returns false if no more data to read, true otherwise.
//--------------------------------------------------------------
bool gvfPianoInputs::update(){
    
    if (kinect_is_live) {
        kinect_input.update();
        current_point = kinect_input.get_data();
         
        if (kinect_input.get_state() == nite::SKELETON_TRACKED) {

            // !!!: Dirty scaling
            leftHand = current_point.joints[nite::JOINT_LEFT_HAND];
            rightHand = current_point.joints[nite::JOINT_RIGHT_HAND];
            head = current_point.joints[nite::JOINT_HEAD];
        }
        
    }
    
    // Get data from 3D accelerometer.
    while (accReceiver.hasWaitingMessages()) {
        accData.at(0) = getAccData(accReceiver, "/wax/14", 0);
    }
    
    if (is_reading) {
        return ReadCsvData();
    }
    else {
        storeInput();
        if (is_writing) {
            WriteCsvData(&csv_recorder);
        }
        return true;
    }
}

void gvfPianoInputs::storeInput() {
    // Store in input vector.
    int inputIterator = 0;
    if (accOneOn) {
        for (int i = 0; i < 3; ++i) {
            inputData.at(i + inputIterator) = (accData.at(0)).at(i);
        }
        inputIterator += 3;
    }
    if (accTwoOn) {
        for (int i = 0; i < 3; ++i) {
            inputData.at(i + inputIterator) = (accData.at(1)).at(i);
        }
        inputIterator += 3;
    }
    if (leftHandOn) {
        for (int i = 0; i < 3; ++i) {
            inputData.at(i + inputIterator) = leftHand[i];
        }
        inputIterator += 3;
    }
    if (rightHandOn) {
        for (int i = 0; i < 3; ++i) {
            inputData.at(i + inputIterator) = rightHand[i];
        }
        inputIterator += 3;
    }
}

// Returns initial point.
//--------------------------------------------------------------
std::vector<float> gvfPianoInputs::initialise() {
    initial_point = inputData;
    storeInput(); // Update with initial point.
    return initial_point;
}

KinectInput* gvfPianoInputs::get_kinect_input() {
    return &kinect_input;
}

//--------------------------------------------------------------
std::vector<float> gvfPianoInputs::getInputData() {
    
    return inputData;
}

int gvfPianoInputs::getInputSize() {
    
    return inputDataSize;
}

//--------------------------------------------------------------
void gvfPianoInputs::setInputs(bool _accOneOn, bool _accTwoOn,
                               bool _leftHandOn, bool _rightHandOn) {
    accOneOn = _accOneOn;
    accTwoOn = _accTwoOn;
    leftHandOn = _leftHandOn;
    rightHandOn = _rightHandOn;
    
    // TODO: Replace by iterating through inputsInfo vector
    inputDataSize = 3 * (leftHandOn + rightHandOn + accOneOn + accTwoOn);
    cout << "input size " << inputDataSize << endl;
    inputData = std::vector<float>(inputDataSize, 0.0);
    initial_point = std::vector<float>(inputDataSize, 0.0);
    
}

//MARK: Data Input
//--------------------------------------------------------------
//Data Input: Accelerometer Data
std::vector<float> gvfPianoInputs::getAccData(ofxOscReceiver& accReceiver, string address, int accId) {
    
    ofxOscMessage accMessage;
    
    std::vector<float> accVector = vector<float>(3,0.0);
    
    accReceiver.getNextMessage(&accMessage);
    
    if (accMessage.getAddress() == address) {
        for (int i = 0; i < 3; i++)
            accVector.at(i) = accMessage.getArgAsFloat(i);
    }
    else {
        return accData.at(accId);
    }
    
    return accVector;
    
}



//--------------------------------------------------------------
//--------------------------------------------------------------
// MARK: Data Output
//--------------------------------------------------------------
//--------------------------------------------------------------

// MARK: Writing CSV
void gvfPianoInputs::StartFile() {
    
    csv_recorder.clear();
    csv_recorder.createFile(ofToDataPath("new_file.csv"));
    csv_recorder.fileSeparator = ' ';
    
    is_writing = true;
}

void gvfPianoInputs::EndFile() {
    
    csv_recorder.saveFile(ofToDataPath("gesture_saved.csv"));
    
    is_writing = false;
}

void gvfPianoInputs::WriteCsvData(wng::ofxCsv* _csv_recorder) {
    
    // TODO: Add metadata about each gesture.
    
    int row = _csv_recorder->numRows;
    
    _csv_recorder->setInt(row, 0, row);
    
    // Write input data.
    for (int i = 0; i < inputDataSize; ++i)
        _csv_recorder->setFloat(row, i + 1, inputData[i]);
    
    // Write Recognition Information
    
}

//--------------------------------------------------------------
//--------------------------------------------------------------
// MARK: Reading CSV
void gvfPianoInputs::LoadFile(string filename) {
    
    csv_reader.loadFile(ofToDataPath(filename), " ");
    
    csv_row = 0;
    is_reading = true;
    
}



// Reads CSV file, returns false when done.
bool gvfPianoInputs::ReadCsvData() {
    
    if (csv_row >= csv_reader.numRows) {
        is_reading = false;
    }
    else {
        
        int size = csv_reader.data[csv_row].size();
        
        inputData = vector<float>(size - 1);
        // Read to input Data
        for (int i = 1; i < size; ++i) {
            inputData[i - 1] = csv_reader.getFloat(csv_row, i);
        }
        
        ++ csv_row;
    }
    return is_reading;
    
    
}
