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

void piafOSCSender::SendInfo(int info, string address) {
    
    ofxOscMessage message;
    
    message.setAddress(address);
    message.addIntArg(info);
    
    sender.sendMessage(message);

}

void piafOSCSender::SendInfo(float info, string address) {
    
    ofxOscMessage message;
    
    message.setAddress(address);
    message.addFloatArg(info);
    
    sender.sendMessage(message);
    
}

void piafOSCSender::SendDataPoint(int point_id, string data_name, ofVec3f point) {
    
    ofxOscMessage data_point;
    
    data_point.setAddress("/data");
    data_point.addIntArg(point_id);
    data_point.addStringArg(data_name);
    
    for (int i = 0; i < 3; ++i) {
        data_point.addFloatArg(point[i]);
    }
    
    sender.sendMessage(data_point);
}

void piafOSCSender::SendDataPoint(int point_id, const RigidBody& body_part) {
    
    ofxOscMessage data_point;
    
    data_point.setAddress("/data");
    data_point.addIntArg(point_id);
    data_point.addStringArg(body_part.name);
    
    for (int i = 0; i < 3; ++i) {
        data_point.addFloatArg(body_part.position[i]);
    }
    for (int i = 0; i < 4; ++i) {
        data_point.addFloatArg(body_part.orientation[i]);
    }
    
    sender.sendMessage(data_point);
}

void piafOSCSender::SendGVFOutcome(int gesture_id, ofxGVFEstimation estimation) {
    
    ofxOscMessage gvf_outcome;
    
    gvf_outcome.setAddress("/estimation");
    gvf_outcome.addIntArg(gesture_id);
    
    gvf_outcome.addFloatArg(estimation.probability);
    gvf_outcome.addFloatArg(estimation.phase);
    gvf_outcome.addFloatArg(estimation.speed);

    gvf_outcome.addIntArg(estimation.scale.size());
    for (int i = 0; i < estimation.scale.size(); ++i) {
        gvf_outcome.addFloatArg(estimation.scale[i]);
    }
    
    gvf_outcome.addIntArg(estimation.rotation.size());
    for (int i = 0; i < estimation.rotation.size(); ++i) {
        gvf_outcome.addFloatArg(estimation.rotation[i]);
    }

    // Padding option
//    for (int i = 0; i < 3; ++i) {
//        if (i < estimation.scale.size())
//            gvf_outcome.addFloatArg(estimation.scale[i]);
//        else
//            gvf_outcome.addFloatArg(0.); // padding
//    }
//    
//    gvf_outcome.addIntArg(estimation.rotation.size());
//    for (int i = 0; i < 3; ++i) {
//        if (i < estimation.rotation.size())
//            gvf_outcome.addFloatArg(estimation.rotation[i]);
//        else
//            gvf_outcome.addFloatArg(0.); // padding
//    }
    
    sender.sendMessage(gvf_outcome);
    
}
