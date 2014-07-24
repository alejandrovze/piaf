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
    
    SetupSkeleton();
    
    initColors();
    
    kinect_image = kinect_input->GetImage();
    
    // TODO: Fine-Tune Camera Setup
    cam.enableMouseInput();
    cam.setAutoDistance(false);
    cam.setDistance(100);
    cam.setOrientation(ofQuaternion(0, -1, 0, 0));
}

//--------------------------------------------------------------
void KinectDisplay::Draw(SkeletonDataPoint skeleton, vector<int> joints_on) {
    ofPushMatrix();
	cam.begin();
    
    for (int i = 0; i < NITE_JOINT_COUNT; ++i)
        skeleton_mesh.setVertex(i, skeleton.positions[i]);
    
    ofSetLineWidth(5);
    
    skeleton_mesh.draw();
    
    ofSetColor(0, 0, 255);
    
    // Add circle for Joints ON.
    for (int i = 0; i < joints_on.size(); ++i) {
        ofCircle(skeleton_mesh.getVertex(joints_on[i]), 8);
    }
    
    DrawGesture();
    DrawTemplates();
    
    cam.end();
    ofPopMatrix();
    
}

//--------------------------------------------------------------
//// Draw each saved template, with phase if following
//--------------------------------------------------------------
// TODO: generalize (currently 1 3d point)
void KinectDisplay::DrawTemplates() {
    
    
    for (int i = 0; i < gvf_handler->getNumberOfGestureTemplates(); ++i) {
        
        ofMesh& representation = gvf_handler->getGestureTemplate(i).getRepresentationsRaw()[0][0];
        
        ofColor color = colors[i];
        
        ofPushMatrix();
        
        if (gvf_handler->getState() == ofxGVF::STATE_FOLLOWING) {
            
            representation.enableColors();
        
            float phase = gvf_handler->getTemplateRecogInfo(i).phase;
            float probability = gvf_handler->getTemplateRecogInfo(i).probability;
            
            if (phase >= 1.)
                phase = 1.;
            if (phase < 0.)
                phase = 0.;
            if (isnan(phase)) {
                cout << "gotnan phase\n"; //FIXME: Why do we get NANs?
                phase = 0.;
            }
            
            float n_vertices = (float) representation.getNumColors();
            int end_index = (int) (phase * n_vertices);
            
            if (end_index < 0)
                cout << "i " << i << " end " << end_index << " phase " << phase << " prob " << probability << endl;
            
            ofSetLineWidth(0.5 + 10 * probability);
            
            ofColor redness = ofColor(255, 0, 0) * probability + color * (1 - probability);
            
            for (int j = 0; j < end_index; ++j) {
                representation.setColor(j, redness);
            }
            
            representation.draw();
            
            for (int j = end_index; j < n_vertices; ++j) {
                representation.setColor(j, color);
            }
            
            ofSetLineWidth(1);

        }
        else {
            representation.disableColors();
            ofSetColor(color);
            representation.draw();
        }
        
        
        ofPopMatrix();
        
    }
    
}

//--------------------------------------------------------------
// TODO: generalize (currently 1 3d point)
void KinectDisplay::DrawGesture() {
    
    if (gvf_handler->getCurrentGesture()->getNumberOfTemplates() > 0) {
        ofMesh representation = gvf_handler->getCurrentGesture()->getRepresentationsRaw()[0][0];
        
        representation.disableColors();
        
        if (gvf_handler->getIsPlaying()) {
            if ((gvf_handler->getState() == ofxGVF::STATE_FOLLOWING) && (gvf_handler->getMostProbableGestureIndex() > -1))
                ofSetColor(colors[gvf_handler->getMostProbableGestureIndex()]);
            else
                ofSetColor(255, 255, 255);
        }
        else
            ofSetColor(50, 50, 50);
        
        representation.draw();
    }

}


//--------------------------------------------------------------
void KinectDisplay::DrawKinect(SkeletonDataPoint skeleton) {

    if (kinect_input->get_is_running()) {
        
        ofScale(ofGetWindowWidth() / (float)kinect_image->getWidth(), ofGetWindowHeight() / (float) kinect_image->getHeight());
        
        // If tracked, dim the image.
        if (skeleton.state != nite::SKELETON_TRACKED) {
            skeleton_mesh.disableColors();
            ofSetColor(50, 50, 60);
        }
        else {
            skeleton_mesh.enableColors();
            ofSetColor(150, 150, 180);
        }
        
        kinect_image->draw(0, 0);
        
        for (int i = 0; i < NITE_JOINT_COUNT; ++i) {
            
            ofPoint depth_pt = ScaleToKinect(skeleton.positions[i]);
            
            skeleton_mesh.setVertex(i, depth_pt);
            
        }
        
    }
    
}

// ???: Probably unnecessary
//--------------------------------------------------------------
void KinectDisplay::drawInteractionArea(){
	ofRectangle vp = ofGetCurrentViewport();
	float r = MIN(vp.width, vp.height) * 0.5f;
	float x = vp.width * 0.5f;
	float y = vp.height * 0.5f;
	
	ofPushStyle();
	ofSetLineWidth(3);
	ofSetColor(255, 255, 0);
	ofNoFill();
	glDepthMask(false);
	ofCircle(x, y, r);
	glDepthMask(true);
	ofPopStyle();
}

//--------------------------------------------------------------
void KinectDisplay::SetupSkeleton() {
    
    skeleton_mesh.setMode(OF_PRIMITIVE_LINES);
    skeleton_mesh.enableColors();
    skeleton_mesh.enableIndices();
    
    for (int i = 0; i < NITE_JOINT_COUNT; ++i) {
        
        skeleton_mesh.addVertex(ofPoint(0, 0, 0));
        
        if (i <= NITE_JOINT_TORSO)
            skeleton_mesh.addColor(ofColor(255, 0, 255));
        else
            skeleton_mesh.addColor(ofColor(30, 0, 30));
        
    }
    
    for (int i = 0; i < NITE_JOINT_CONNECTIONS_COUNT; ++i) {
        skeleton_mesh.addIndex(NiteJointConnections[i][0]);
        skeleton_mesh.addIndex(NiteJointConnections[i][1]);
    }
}

//--------------------------------------------------------------
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
    colors.push_back(ofColor::green);
    colors.push_back(ofColor::violet);
    colors.push_back(ofColor::orange);
    colors.push_back(ofColor::yellow);
    colors.push_back(ofColor::pink);
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



//--------------------------------------------------------------
//--------------------------------------------------------------
//--------------------------------------------------------------

// FIXME: Fix drawing of particles.
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
            float radius = weights[i] / weightAverage;
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