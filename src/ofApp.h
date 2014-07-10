//
//  Adapted by Alejandro Van Zandt-Escobar
// Updated for openFrameworks 0.8


#pragma once

#include "ofMain.h"

#include "GVFHandler.h"

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
    GVFHandler     handler;
    gvfPianoInputs      inputs;
    
    piafOSCSender sender;
    
    void LoadInputFile();
    
};

