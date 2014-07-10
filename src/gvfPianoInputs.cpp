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
    
    // KINET SETUP
    kinect_input.setup();
    kinect_joints = vector<ofVec3f>(NITE_JOINT_COUNT);
    joints_on.push_back(NITE_JOINT_LEFT_HAND);
    
    // WAX ACC SETUP
    accelerometers.push_back(WaxAccInput(8200, 14));
    acc_data = vector<ofVec3f>(accelerometers.size());
    
    // CSV File business
    is_writing = false;
    is_reading = false;
    file_num = 0;
    
}

// Returns false if no more data to read, true otherwise.
//--------------------------------------------------------------
bool gvfPianoInputs::update(){
    
    if (is_reading) {
        if(!ReadCsvData())
            return false; // Stop playing if we get to the end of the data.
    }
    else {
        // Update Kinect
        if (kinect_input.get_is_running()) {
            kinect_input.update();
            kinect_joints = kinect_input.get_data().positions; // !!!: Later get all the data?
        }
        // Update Wax
        for (int i = 0; i < accelerometers.size(); ++i) {
            accelerometers[i].update();
            acc_data[i] = accelerometers[i].get_data();
        }
    }
    
    if (is_writing) {
        WriteCsvData(&csv_recorder);
    }
    
    return true;
}

//--------------------------------------------------------------
vector<float> gvfPianoInputs::ConcatenateInput() {
    
    vector<float> input_data;
    
    // Store in input vector.
    for(int i = 0; i < joints_on.size(); ++i) {
            for (int j = 0; j < 3; ++j) {
                input_data.push_back(kinect_joints[joints_on[i]][j]);
            }
    }
    
    for (int i = 0; i < acc_on.size(); ++i) {
        ofVec3f acc_data = accelerometers[acc_on[i]].get_data();
        for (int j = 0; j < 3; ++j)
            input_data.push_back(acc_data[i]);
    }
    
    return input_data;

}

int gvfPianoInputs::get_input_size() {
    
    return 3 * (joints_on.size() + acc_on.size());
    
}

//--------------------------------------------------------------
void gvfPianoInputs::set_joints_on(vector<int> _joints_on) {
    
    for (int i = 0; i < _joints_on.size(); ++i)
        assert(_joints_on[i] < NITE_JOINT_COUNT && _joints_on[i] >= 0);
    
    joints_on = _joints_on;
}

//--------------------------------------------------------------
vector<int> gvfPianoInputs::get_joints_on() {
    return joints_on;
}

//--------------------------------------------------------------
void gvfPianoInputs::set_acc_on(vector<int> _acc_on) {
    
    for (int i = 0; i < _acc_on.size(); ++i)
        assert(_acc_on[i] < accelerometers.size() && _acc_on[i] >= 0);
    
    acc_on = _acc_on;
}

//--------------------------------------------------------------
vector<int> gvfPianoInputs::get_acc_on() {
    return acc_on;
}

// !!!: Check
//--------------------------------------------------------------
KinectInput* gvfPianoInputs::get_kinect_input() {
    return &kinect_input;
}

// !!!: Check
//--------------------------------------------------------------
std::vector<ofPoint> gvfPianoInputs::GetKinectData() {
    return kinect_joints;
}

// !!!: Check
//--------------------------------------------------------------
vector<WaxAccInput>* gvfPianoInputs::GetAccInputs() {
    return &accelerometers;
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
        for (int j = 0; j < 3; ++j) {
            _csv_recorder->setFloat(row, i * 3 + j + position, kinect_joints[i][j]);
        }
    }
    
    position += NITE_JOINT_COUNT * 3;
    
    // Write Acc Data
    for (int i = 0; i < accelerometers.size(); ++i) {
        ofVec3f acc_data = accelerometers[i].get_data();
        for (int j = 0; j < 3; ++j)
            _csv_recorder->setFloat(row, position + i * 3 + j, acc_data[i]);
    }
    
    position += accelerometers.size() * 3;
    
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
        
        int size = csv_reader.data[csv_row].size();
        
        int position = 1;
        
        // Read Kinect
        for (int i = 0; i < NITE_JOINT_COUNT; ++i) {
            for (int j = 0; j < 3; ++j) {
                kinect_joints[i][j] = csv_reader.getFloat(csv_row, i * 3 + position + j);
            }
        }
    
        position += 3 * NITE_JOINT_COUNT;
        
        // Read Acc
        for (int i = 0; i < acc_data.size(); ++i) {
            for (int j = 0; j < 3; ++j) {
                acc_data[i][j] = csv_reader.getFloat(csv_row, i * 3 + position + j);
            }
        }
                
        ++ csv_row;
    }
    return is_reading;
    
}
