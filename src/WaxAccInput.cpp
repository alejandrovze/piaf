//
//  WaxAccInput.cpp
//  piaf
//
//  Created by Alejandro Van Zandt-Escobar on 09/07/2014.
//
//

#include "WaxAccInput.h"

WaxAccInput::WaxAccInput(int wax_id) {
    
    // Accelerometer setup.
    id = wax_id;
     
    address = "/wax/" + ofToString(id);
    
    acc_data = ofVec3f(0., 0., 0.);
    
}

void WaxAccInput::update(ofxOscMessage& acc_message) {
    if (acc_message.getAddress() == address) {
        for (int i = 0; i < 3; i++)
            acc_data[i] = acc_message.getArgAsFloat(i);
    }
    
}

ofVec3f WaxAccInput::get_data() {
    return acc_data;
}

int WaxAccInput::get_id() {
    return id;
}

string WaxAccInput::get_address() {
    return address;
}