//
//  ofxUIMultiMovingGraph.h
//  piaf
//
//  Created by Alejandro Van Zandt-Escobar on 15/07/2014.
//
//

#ifndef __piaf__ofxUIMultiMovingGraph__
#define __piaf__ofxUIMultiMovingGraph__

#include "ofxUIWidget.h"
#include "ofxUIDefines.h"


class ofxUIMultiMovingGraph : public ofxUIWidget
{
public:
    ofxUIMultiMovingGraph(float x, float y, float w, float h, vector<ofVec3f> _buffer, int _bufferSize, float _min, float _max, string _name);
    ofxUIMultiMovingGraph(float w, float h, vector<ofVec3f> _buffer, int _bufferSize, float _min, float _max, string _name);
    
    void init(float x, float y, float w, float h, vector<ofVec3f> _buffer, int _bufferSize, float _min, float _max, string _name);
    
    virtual void drawFill();
	void setParent(ofxUIWidget *_parent);
    void addPoint(ofVec3f _point);
    vector<ofVec3f> &getBuffer();
    void setBuffer(vector<ofVec3f> _buffer);
    void setMax(float _max);
    float getMax();
    void setMin(float _min);
    float getMin();
    ofVec2f getMaxAndMind();
    void setMaxAndMin(float _max, float _min);
    
protected:    //inherited: ofxUIRectangle *rect; ofxUIWidget *parent;
    vector<ofVec3f> buffer;
    float max, min, scale, inc;
    unsigned int bufferSize;
    
private:
    void drawLine(ofColor line_color, int dim);
    
};

#endif /* defined(__piaf__ofxUIMultiMovingGraph__) */
