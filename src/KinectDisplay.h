//
//  KinectDisplay.h
//  piaf
//
//  Created by Alejandro Van Zandt-Escobar on 10/07/2014.
//
//

#ifndef __piaf__KinectDisplay__
#define __piaf__KinectDisplay__

#include <iostream>

#include "KinectInput.h"
#include "GVFHandler.h"

class KinectDisplay {
    
public:
    
    void setup(KinectInput* input, GVFHandler* handler);
    
    void draw(int x, int y, int _width, int _height);
    
private:
    
    
    KinectInput* kinect_input;
    GVFHandler* gvf_handler;
    
    ofImage* kinect_image;
    
    void DrawSkeleton(vector<ofPoint> skeleton);
    void DrawTemplates();
    void DrawGesture();
    ofPoint ScaleToKinect(ofPoint init_point);
    void DrawParticles();
    
    
    
    // COLORS
    void initColors();
    ofColor generateRandomColor();
    std::vector<ofColor> colors;
    
};


#endif /* defined(__piaf__KinectDisplay__) */
