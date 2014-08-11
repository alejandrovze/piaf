//
//  piafOSCSender.h
//  piaf
//
//  Created by Alejandro Van Zandt-Escobar on 24/06/2014.
//
//

#ifndef __piaf__piafOSCSender__
#define __piaf__piafOSCSender__

#include <iostream>
#include "ofxOsc.h"
#include "ofxGVF.h"
#include "gvfPianoInputs.h"

#define HOST "localhost"
#define OUT_PORT 7272

//--------------------------------------------------------
class piafOSCSender {
    
public:
    
    void setup();
    
    void SendGVFOutcome(int gesture_id, ofxGVFEstimation estimation);
    void SendDataPoint(int point_id, string data_name, ofVec3f point);
    void SendDataPoint(int point_id, const RigidBody& body_part);
    
    void SendInfo(int info, string address);
    void SendInfo(float info, string address);
    
private:
    
    ofxOscSender sender;
    
};




#endif /* defined(__piaf__piafOSCSender__) */
