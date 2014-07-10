//
//  KinectDisplay.cpp
//  piaf
//
//  Created by Alejandro Van Zandt-Escobar on 10/07/2014.
//
//

#include "KinectDisplay.h"

void KinectDisplay::setup(KinectInput* input, GVFHandler* handler) {
    
    kinect_input = input;
    gvf_handler = handler;
    
    initColors();
    
    kinect_image = kinect_input->GetImage(); // TODO: Fix path to image.
    
}


//--------------------------------------------------------------
//// Draw kinect templates and skeleton display
//--------------------------------------------------------------
void KinectDisplay::draw(int x, int y, int _width, int _height) {
    
    ofPushMatrix();
    
    ofTranslate(x, y);
    
    float width = _width;
    float height = _height;
    
    if (kinect_input->get_is_running()) {
        
        kinect_image->draw(x, y, width, height);
        
        width /= (float) kinect_image->getWidth();
        height /= (float) kinect_image->getHeight();
    }
    
    //    DrawSkeleton(inputs->GetKinectData());
    
    ofScale(width, height);
    ofPopMatrix();
    
    //    if (inputs->get_kinect_input()->get_state() == nite::SKELETON_TRACKED)
    
    ofMatrix4x4 matrix = ofGetCurrentMatrix(OF_MATRIX_MODELVIEW);
    
    
    ofPushMatrix();
    ofTranslate(600, 500);
    
    if (gvf_handler->getCurrentGesture()->getNumberOfTemplates() > 0) {
        
        gvf_handler->getCurrentGesture()->draw();
        
        //        DrawGesture();
    }
    
    //    for(int i = 0; i < gvf_handler->getNumberOfGestureTemplates(); i++){
    //
    //        ofxGVFGesture & gestureTemplate = gvf_handler->getGestureTemplate(i);
    //
    //        ofPushMatrix();
    //        gestureTemplate.draw(i * 100.0f, 0, 100.0f, 100.0f);
    //        ofPopMatrix();
    //
    //    }
    //
    DrawParticles();
    
    
    
    ofPopMatrix();
    
    
    //    DrawTemplates();
    
    
}

void KinectDisplay::DrawParticles() {
    
    vector< vector<float> > pp = gvf_handler->getParticlesPositions();
    
    int ppSize = pp.size();
    float scale = 1;
    
    if(ppSize > 0 && gvf_handler->getCurrentGesture()->getNumberOfTemplates() > 0){
        // as the colors show, the vector returned by getG()
        // does not seem to be in synch with the information returned by particlesPositions
        vector<int> gestureIndex = gvf_handler->getG();
        vector<float> weights = gvf_handler->getW();
        
        ofFill();
        
        float weightAverage = getMeanVec(weights);
        
        ofPoint offset = ofPoint(gvf_handler->getCurrentGesture()->getTemplateNormal()[0][0] - pp[0][0], gvf_handler->getCurrentGesture()->getTemplateNormal()[0][1] - pp[0][1]);
        
        for(int i = 0; i < ppSize; i++){
            
            // each particle position is retrieved
            ofPoint point(pp[i][0], pp[i][1]);
            
            // and then scaled and translated in order to be drawn
            float x = ((point.x)) * (gvf_handler->getCurrentGesture()->getMaxRange()[0] - gvf_handler->getCurrentGesture()->getMinRange()[0]);
            float y = ((point.y)) * (gvf_handler->getCurrentGesture()->getMaxRange()[1] - gvf_handler->getCurrentGesture()->getMinRange()[1]);
            
            
            // the weight of the particle is normalised
            // and then used as the radius of the circle representing the particle
            float radius = weights[i]/weightAverage;
            ofColor c = ofColor(127, 0, 0);
            
            c.setBrightness(198);
            ofSetColor(c);
            ofPushMatrix();
            ofTranslate(gvf_handler->getCurrentGesture()->getInitialObservationRaw()[0],gvf_handler->getCurrentGesture()->getInitialObservationRaw()[1]);
            //ofCircle(x, y, radius);
            ofCircle(x, y, 1); // MATT something wrong with radius above
            ofPopMatrix();
            
        }
    }
    
}

// FIXME: Gestures of more than one dimension.
void KinectDisplay::DrawGesture() {
    
    
    //    ofPushMatrix();
    //    ofPopMatrix();
    
    vector< vector<float> > template_data = gvf_handler->getCurrentGesture()->getTemplateRaw();
    vector<float> initial = gvf_handler->getCurrentGesture()->getInitialObservationRaw();
    
    int length = template_data.size();
    
    ofPolyline line;
    
    for (int t = 0; t < length; ++t) {
        
        int dimension = gvf_handler->getCurrentGesture()->getNumberDimensions();
        assert(dimension == 3);
        
        ofPoint orig_pos = ofPoint(template_data[t][0] + initial[0], template_data[t][1] + initial[1], template_data[t][2] + initial[2]);
        
        ofPoint pos = ScaleToKinect(orig_pos);
        
        ofSetLineWidth(1);
        
        ofSetColor(ofColor(0, 255, 0));
        ofFill();
        
        line.addVertex(pos);
        ofCircle(pos, 5);
        
    }
    
    line.draw();
    
    
    ofSetColor(255, 255, 255);
    ofNoFill();
    
    
}

//--------------------------------------------------------------
//// Draw each saved template, with phase if following
//--------------------------------------------------------------
void KinectDisplay::DrawTemplates() {
    
    int n_templates = gvf_handler->getNumberOfGestureTemplates();
    
    for (int i = 0; i < n_templates; ++i) {
        
        vector< vector<float> > template_data = gvf_handler->getGestureTemplate(i).getTemplateRaw();
        vector<float> initial = gvf_handler->getGestureTemplate(i).getInitialObservationRaw();
        
        int length = template_data.size();
        
        ofPolyline line;
        
        for (int t = 0; t < length; ++t) {
            
            int dimension = gvf_handler->getGestureTemplate(i).getNumberDimensions();
            assert(dimension == 3);
            
            ofPoint orig_pos = ofPoint(template_data[t][0] + initial[0], template_data[t][1] + initial[1], template_data[t][2] + initial[2]);
            
            ofPoint pos = ScaleToKinect(orig_pos);
            
            if (gvf_handler->getState() == ofxGVF::STATE_FOLLOWING && gvf_handler->getIsPlaying()) {
                
                // Set Color according to Probability
                // FIXME: quick fix for getOutcomes crashing
                float probability;
                if (gvf_handler->getMostProbableGestureIndex() > -1) {
                    probability = gvf_handler->getOutcomes().allProbabilities[i];
                }
                else {
                    probability = 0;
                }
                ofColor template_color = ofColor(255, 0, 0) * probability + colors[i] * (1 - probability); // FIXME: unsafe
                
                ofSetColor(template_color);
                
                ofSetLineWidth(0.5 + 10 * probability);
                
                // Set Fill according to Phase
                float phase = 0;
                
                phase = gvf_handler->getOutcomes().allPhases[i];
                
                if (phase >= ((float) t / (float) length)) {
                    ofFill();
                }
                else {
                    ofNoFill();
                }
                
            }
            else {
                
                ofSetLineWidth(1);
                ofSetColor(colors[i]);
                ofNoFill();
            }
            // Fill according to phase reading.
            
            line.addVertex(pos);
            ofCircle(pos, 5);
            
        }
        line.draw();
        
        
        ofSetColor(255, 255, 255);
        ofNoFill();
        
        
        
    }
    
    
}

//--------------------------------------------------------------
//// Live skeleton display
//--------------------------------------------------------------
void KinectDisplay::DrawSkeleton(vector<ofPoint> skeleton) {
    
    int x_pos;
    int y_pos;
    
    ofFill();
    
    ofSetColor(0, 0, 255);
    
    
    for (int i = 0; i <= NITE_JOINT_TORSO; ++i) {
        
        ofPoint depth_pt = ScaleToKinect(skeleton[i]);
        
        ofCircle(depth_pt, 1);
        
    }
    
    ofSetColor(255, 255, 255);
    
    ofNoFill();
    
}

ofPoint KinectDisplay::ScaleToKinect(ofPoint init_point) {
    
    ofPoint point;
    
    if (kinect_input->get_is_running()) {
        
        // FIXME: Convert World to Depth without Kinect plugged in?
        point = kinect_input->WorldToDepth(init_point);
        
        return point;
    }
    
    else {
        // TODO: how to deal with this?
        return init_point;
    }
    
}

// COLORS
//--------------------------------------------------------------
void KinectDisplay::initColors()
{
    colors.clear();
    colors.push_back(ofColor::blue);
    colors.push_back(ofColor::cyan);
    colors.push_back(ofColor::aqua);
    colors.push_back(ofColor::violet);
    colors.push_back(ofColor::aliceBlue);
    colors.push_back(ofColor::aquamarine);
    colors.push_back(ofColor::azure);
}

//--------------------------------------------------------------
ofColor KinectDisplay::generateRandomColor()
{
    ofColor c;
    
    if(colors.size() == 0)
        initColors();
    
    int colorsRemaining = colors.size();
    
    int index = ofRandom(0, colorsRemaining - 1);
    
    c = colors[index];
    colors.erase(colors.begin() + index);
    return c;
}

