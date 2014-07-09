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
    
    current_point = vector<ofVec3f>(NITE_JOINT_COUNT);
    kinect_joints_on = vector<bool>(NITE_JOINT_COUNT, false);
    n_joints = 0;
    kinect_joints_on[NITE_JOINT_LEFT_HAND] = true;
    ++n_joints;
    
    // Accelerometer setup.
    int port = 8200;
    accReceiver.setup(port);
    accAddresses.push_back("/wax/14");
    accAddresses.push_back("/wax/21");
    
    // Initialize accelerometer input vector
    accData = std::vector<vector<float> >(1, vector<float>(3,0.0));
    accOneOn = false;
    accTwoOn = false;
    
    inputDataSize = 3 * (accOneOn + accTwoOn + n_joints);
    
    // CSV File business
    is_writing = false;
    is_reading = false;
    file_num = 0;
    
}

// Returns false if no more data to read, true otherwise.
//--------------------------------------------------------------
bool gvfPianoInputs::update(){
    
    if (kinect_is_live) {
        
        kinect_input.update();
        current_point = kinect_input.get_data().joints;
        // ???: Should we stop playing if tracking stops?
    }
    
    // Get data from 3D accelerometer.
    while (accReceiver.hasWaitingMessages()) {
        accData.at(0) = getAccData(accReceiver, "/wax/14", 0);
    }
    
    if (is_reading) {
        if(!ReadCsvData())
            return false; // Stop playing if we get to the end of the data. 
    }
    
    storeInput();
    
    if (is_writing) {
        WriteCsvData(&csv_recorder);
    }
    
    return true;
}

void gvfPianoInputs::storeInput() {
    
    inputData.clear();
    
    // Store in input vector.
    for(int i = 0; i < NITE_JOINT_COUNT; ++i) {
        
        if (kinect_joints_on[i]) {
            for (int j = 0; j < 3; ++j) {
                inputData.push_back(current_point[i][j]);
            }
        }
    }

    if (accOneOn) {
        for (int i = 0; i < 3; ++i) {
            inputData.push_back(accData[0][i]);
        }
    }
    if (accTwoOn) {
        for (int i = 0; i < 3; ++i) {
            inputData.push_back(accData[1][i]);
        }
    }
}


//--------------------------------------------------------------
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
void gvfPianoInputs::SetKinectJoints(vector<int> joints_on) {
    
    kinect_joints_on = vector<bool>(NITE_JOINT_COUNT, false);
    
    for (int i = 0; i < joints_on.size(); ++i)
        kinect_joints_on[joints_on[i]] = true;
    
    n_joints = joints_on.size();
    
    inputDataSize = 3 * (accOneOn + accTwoOn + n_joints);
}

//--------------------------------------------------------------
vector<bool> gvfPianoInputs::GetKinectJoints() {
    return kinect_joints_on;
}

//--------------------------------------------------------------
std::vector<ofPoint> gvfPianoInputs::GetKinectData() {
    
    return current_point;
}

//--------------------------------------------------------------
void gvfPianoInputs::SetAccInputs(bool accOn, int acc) {
    
    
    if (acc == 1)
        accOneOn == accOn;
    else if (acc == 2)
        accTwoOn == accOn; 
    
    inputDataSize = 3 * (accOneOn + accTwoOn + n_joints);
}

//--------------------------------------------------------------
vector<bool> gvfPianoInputs::GetAccInputs() {
    
    vector<bool> acc_inputs;
    acc_inputs.push_back(accOneOn);
    acc_inputs.push_back(accTwoOn);
    
    return acc_inputs;
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
    string filename = "gesture-" + ofToString(file_num) + ".csv";
    csv_recorder.createFile(ofToDataPath(filename));
    csv_recorder.fileSeparator = ' ';
    
    is_writing = true;
}

void gvfPianoInputs::EndFile() {
    
    string filename = "gesture-" + ofToString(file_num) + ".csv";
    
    csv_recorder.saveFile(ofToDataPath(filename));
    
    ++ file_num;
    is_writing = false;
}

void gvfPianoInputs::WriteCsvData(wng::ofxCsv* _csv_recorder) {
    
    // TODO: Add metadata about each gesture.
    
    int row = _csv_recorder->numRows;
    
    _csv_recorder->setInt(row, 0, row);
    
    int position = 1;
    // Write Kinect
    for (int i = 0; i < NITE_JOINT_COUNT; ++i) {
        _csv_recorder->setFloat(row, i * 3 + position, current_point[i].x);
        _csv_recorder->setFloat(row, i * 3 + 1 + position, current_point[i].y);
        _csv_recorder->setFloat(row, i * 3 + 2 + position, current_point[i].z);
    }
    
    position += NITE_JOINT_COUNT * 3;
    
    // Write Acc Data
    for (int i = 0; i < accData.size(); ++i) {
        for (int j = 0; j < 3; ++j) {
            _csv_recorder->setFloat(row, position + i * 3 + j, accData[i][j]);
        }
    }
    
    position += accData.size() * 3;
    
}

//--------------------------------------------------------------
//--------------------------------------------------------------
// MARK: Reading CSV
void gvfPianoInputs::LoadFile(string filename) {
    
    csv_reader.clear();
    csv_reader.loadFile(ofToDataPath(filename), " ");
    
    csv_row = 0;
    is_reading = true;
    is_writing = false;
    
}

// Reads CSV file, returns false when done.
bool gvfPianoInputs::ReadCsvData() {
    
    if (csv_row >= csv_reader.numRows) {
        is_reading = false;
    }
    else {
        
        // FIXME
        
        int size = csv_reader.data[csv_row].size();
        
        int position = 1;
        
        // Read Kinect
        for (int i = 0; i < NITE_JOINT_COUNT; ++i) {
            for (int j = 0; j < 3; ++j) {
                current_point[i][j] = csv_reader.getFloat(csv_row, i * 3 + position + j);
            }
        }
    
        position += 3 * NITE_JOINT_COUNT;
        
        // Read Acc
        for (int i = 0; i < accData.size(); ++i) {
            for (int j = 0; j < 3; ++j) {
                accData[i][j] = csv_reader.getFloat(csv_row, i * 3 + position + j);
            }
        }
                
        ++ csv_row;
    }
    return is_reading;
    
    
}
