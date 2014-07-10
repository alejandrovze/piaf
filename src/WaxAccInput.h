//
//  WaxAccInput.h
//  piaf
//
//  Created by Alejandro Van Zandt-Escobar on 09/07/2014.
//
//

#ifndef __piaf__WaxAccInput__
#define __piaf__WaxAccInput__

#include <iostream>

#include "ofMain.h"

#include "ofxOsc.h"

class WaxAccInput {
    
public:
    
    WaxAccInput(int port, int wax_id);
    
    void update();
    
    ofVec3f get_data();
    int get_id();

private:
    
    int id;
    
    string address;
    ofVec3f acc_data;

    ofxOscReceiver acc_receiver;
    
};

#endif /* defined(__piaf__WaxAccInput__) */
