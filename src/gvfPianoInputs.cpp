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
    kinect_data = SkeletonDataPoint();
    joints_on.push_back(NITE_JOINT_LEFT_HAND);
    
    // WAX ACC SETUP
    wax_receiver.setup(8200);
    accelerometers.push_back(new WaxAccInput(14));
    accelerometers.push_back(new WaxAccInput(12));
    acc_data = vector<ofVec3f>(accelerometers.size());
    
    // PIANO INPUT SETUP (MIDI)
    piano.setup();
    
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
            kinect_data = kinect_input.get_data();
        }
        // Update Wax
        while (wax_receiver.hasWaitingMessages()) {
            ofxOscMessage wax_msg;
            wax_receiver.getNextMessage(&wax_msg);
    
            for (int i = 0; i < accelerometers.size(); ++i) {
                accelerometers[i]->update(wax_msg);
                acc_data[i] = accelerometers[i]->get_data();
            }
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
            input_data.push_back(kinect_data.positions[joints_on[i]][j]);
        }
    }
    
    for (int i = 0; i < acc_on.size(); ++i) {
        ofVec3f acc_data = accelerometers[acc_on[i]]->get_data();
        for (int j = 0; j < 3; ++j)
            input_data.push_back(acc_data[i]);
    }
    
    return input_data;

}

// TODO: Make this adjustable (?)
//--------------------------------------------------------------
vector<vector<float> > gvfPianoInputs::get_input_ranges() {
    
    // input_ranges[0] ==> min
    // input_ranges[1] ==> max
    
    vector<vector<float> > input_ranges = vector<vector<float> >(2);
    
    // Kinect Ranges
    for(int i = 0; i < joints_on.size(); ++i) {
        for (int j = 0; j < 2; ++j) {
            input_ranges[0].push_back(-100.);
            input_ranges[1].push_back(100.);
        }
        // Z range is 1000s
        input_ranges[0].push_back(0.);
        input_ranges[1].push_back(2000.);
    }
    
    // Accelerometer Ranges
    for (int i = 0; i < acc_on.size(); ++i) {
        for (int j = 0; j < 3; ++j) {
            input_ranges[0].push_back(-1.);
            input_ranges[1].push_back(1.);
        }
    }
    
    assert(input_ranges[0].size() == get_input_size());
    assert(input_ranges[1].size() == get_input_size());
    
    return input_ranges;
    
}

//--------------------------------------------------------------
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

//--------------------------------------------------------------
KinectInput* gvfPianoInputs::get_kinect_input() {
    return &kinect_input;
}

//--------------------------------------------------------------
SkeletonDataPoint gvfPianoInputs::get_kinect_data() {
    return kinect_data;
}

//--------------------------------------------------------------
vector<WaxAccInput*> gvfPianoInputs::GetAccInputs() {
    return accelerometers;
}

//--------------------------------------------------------------
vector<ofVec3f> gvfPianoInputs::get_acc_data() {
    return acc_data;
}

//--------------------------------------------------------------
ofEvent<ofxMidiMessage>& gvfPianoInputs::GetPianoEvent() {
    return piano.GetPianoEvent();
}

//--------------------------------------------------------------
ofEvent<ofxMidiMessage>& gvfPianoInputs::GetControlEvent() {
    return piano.GetControlEvent();
}

//--------------------------------------------------------------
//--------------------------------------------------------------
// MARK: Data Output
//--------------------------------------------------------------
//--------------------------------------------------------------

// MARK: Writing CSV
//--------------------------------------------------------------
void gvfPianoInputs::StartFile() {
    
    csv_recorder.clear();
    string filename = "gesture-" + ofToString(file_num) + ".csv";
    csv_recorder.createFile(ofToDataPath(filename));
    csv_recorder.fileSeparator = ' ';
    
    is_writing = true;
}

//--------------------------------------------------------------
void gvfPianoInputs::EndFile() {
    
    string filename = "gesture-" + ofToString(file_num) + ".csv";
    
    csv_recorder.saveFile(ofToDataPath(filename));
    
    ++ file_num;
    is_writing = false;
}

//--------------------------------------------------------------
void gvfPianoInputs::WriteCsvData(wng::ofxCsv* _csv_recorder) {
    
    // TODO: Add gesture metadata.
    
    int row = _csv_recorder->numRows;
    
    _csv_recorder->setInt(row, 0, row);
    
    int position = 1;
    
    // Write Kinect
    for (int i = 0; i < NITE_JOINT_COUNT; ++i) {
        for (int j = 0; j < 3; ++j) {
            _csv_recorder->setFloat(row, i * 3 + j + position, kinect_data.positions[i][j]);
        }
    }
    
    position += NITE_JOINT_COUNT * 3;
    
    _csv_recorder->setInt(row, position, kinect_data.state); // Kinect tracking state
    ++position;
    
    // Write Acc Data
    for (int i = 0; i < accelerometers.size(); ++i) {
        ofVec3f acc_data = accelerometers[i]->get_data();
        for (int j = 0; j < 3; ++j)
            _csv_recorder->setFloat(row, position + i * 3 + j, acc_data[j]);
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
                kinect_data.positions[i][j] = csv_reader.getFloat(csv_row, i * 3 + position + j);
            }
        }
    
        position += 3 * NITE_JOINT_COUNT;
        
        kinect_data.state = (nite::SkeletonState) csv_reader.getInt(csv_row, position);
        ++position;
        
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
