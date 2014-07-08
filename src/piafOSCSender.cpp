//
//  piafOSCSender.cpp
//  piaf
//
//  Created by Alejandro Van Zandt-Escobar on 24/06/2014.
//
//

#include "piafOSCSender.h"

//--------------------------------------------------------------
void piafOSCSender::setup(){
    
	// open an outgoing connection to HOST:PORT
	sender.setup(HOST, OUT_PORT);
	cout << "piaf sending osc messages on port " << OUT_PORT << "\n";
    
}

void piafOSCSender::SendGVFOutcome(int gesture_id, float probability, float phase,
                                   float speed, vector<float> scale, vector<float> rotation) {
    
    // Send Data for Most Probable Gesture
        
    ofxOscMessage gvf_outcomes;
    
    gvf_outcomes.setAddress("/gvf_most_probable");
    gvf_outcomes.addIntArg(gesture_id);
    
    gvf_outcomes.addFloatArg(probability);
    gvf_outcomes.addFloatArg(phase);
    gvf_outcomes.addFloatArg(speed);
    
    if (scale.size() != 0)
        gvf_outcomes.addFloatArg(scale[0]);
    
    if (rotation.size() != 0)
        gvf_outcomes.addFloatArg(rotation[0]);
    
//    int scaleDim = scale.size();
//    gvf_outcomes.addIntArg(scaleDim);
//    for (int i = 0; i < scaleDim; ++i)
//        gvf_outcomes.addFloatArg(scale[i]);
//    
//    int rotationDim = rotation.size();
//    gvf_outcomes.addIntArg(rotationDim);
//    for (int i = 0; i < rotationDim; ++i)
//        gvf_outcomes.addFloatArg(rotation[i]);
    
    sender.sendMessage(gvf_outcomes);
    
}

