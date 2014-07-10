//
//  Adapted by Alejandro Van Zandt-Escobar
// Updated for openFrameworks 0.8


#pragma once

#include "ofMain.h"

#include "piafInterface.h"
#include "piafOSCSender.h"

class ofApp : public ofBaseApp {
	
public:
    
    void setup();
    void update();
    void draw();
    
    void exit();
    
    void keyPressed(int key);
    
private:
    
    piafInterface   interface;
    gvfPianoHandler     handler;
    gvfPianoInputs      inputs;
    
    piafOSCSender sender;
    
    void LoadInputFile();
    
};

