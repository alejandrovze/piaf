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

#define HOST "localhost"
#define OUT_PORT 7272

//--------------------------------------------------------
class piafOSCSender {
    
public:
    
    void setup();
    void SendGVFOutcome(int gesture_id, float probability, float phase,
                         float speed, vector<float> scale, vector<float> rotation);
    
private:
    
    ofxOscSender sender;
    
};




#endif /* defined(__piaf__piafOSCSender__) */
