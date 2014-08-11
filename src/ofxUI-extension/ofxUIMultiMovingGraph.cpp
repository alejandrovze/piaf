//
//  ofxUIMultiMovingGraph.cpp
//  piaf
//
//  Created by Alejandro Van Zandt-Escobar on 15/07/2014.
//
//

#include "ofxUIMultiMovingGraph.h"
#include "ofxUI.h"

ofxUIMultiMovingGraph::ofxUIMultiMovingGraph(float x, float y, float w, float h, vector<ofVec3f> _buffer, int _bufferSize, float _min, float _max, string _name) : ofxUIWidget()
{
    init(x, y, w, h, _buffer, _bufferSize, _min, _max, _name);
}

ofxUIMultiMovingGraph::ofxUIMultiMovingGraph(float w, float h, vector<ofVec3f> _buffer, int _bufferSize, float _min, float _max, string _name) : ofxUIWidget()
{
    init(0, 0, w, h, _buffer, _bufferSize, _min, _max, _name);
}

void ofxUIMultiMovingGraph::init(float x, float y, float w, float h, vector<ofVec3f> _buffer, int _bufferSize, float _min, float _max, string _name)
{
    initRect(x,y,w,h);
    name = string(_name);
    kind = OFX_UI_WIDGET_MOVINGGRAPH;
    
    draw_fill = true;
    
    buffer = _buffer;					//the widget's value
    
    bufferSize = _bufferSize;
    max = _max;
    min = _min;
    scale = rect->getHeight()*.5;
    inc = rect->getWidth() / ((float) bufferSize - 1.0);
}

void ofxUIMultiMovingGraph::drawFill()
{
    if(draw_fill)
    {
        ofColor base_color;
        if(draw_fill_highlight)
        {
            base_color = color_fill_highlight;
        }
        else
        {
            base_color = color_fill;
        }
        
        for (int i = 0; i < 3; ++i) {
            // Generate color
            ofColor palette_color = base_color;
            palette_color.setHue(palette_color.getHue() + 0.1 * i * palette_color.limit());
            drawLine(palette_color, i);
        }
    }
}

void ofxUIMultiMovingGraph::drawLine(ofColor line_color, int dim) {
    
    assert(dim >= 0 && dim < 3);
    
    ofNoFill();
    ofxUISetColor(line_color);
    
    ofPushMatrix();
    ofTranslate(rect->getX(), rect->getY()+scale, 0);
    ofSetLineWidth(1.5);
    ofBeginShape();
    for (unsigned int i = 0; i < bufferSize; i++)
    {
        ofVertex(inc*(float)i, ofxUIMap(buffer[i][dim], min, max, scale, -scale, true));
    }
    ofEndShape();
    ofSetLineWidth(1);
    ofPopMatrix();
}

void ofxUIMultiMovingGraph::setParent(ofxUIWidget *_parent)
{
    parent = _parent;
    calculatePaddingRect();
}

void ofxUIMultiMovingGraph::addPoint(ofVec3f _point)
{
    buffer.push_back(_point);
    
    if( buffer.size() >= bufferSize )
    {
        buffer.erase(buffer.begin(), buffer.begin()+1);
    }
}

vector<ofVec3f> &ofxUIMultiMovingGraph::getBuffer()
{
    return buffer;
}

void ofxUIMultiMovingGraph::setBuffer(vector<ofVec3f> _buffer)
{
    buffer = _buffer;
}

void ofxUIMultiMovingGraph::setMax(float _max)
{
    max = _max;
}

float ofxUIMultiMovingGraph::getMax()
{
    return max;
}

void ofxUIMultiMovingGraph::setMin(float _min)
{
    min = _min;
}

float ofxUIMultiMovingGraph::getMin()
{
    return min;
}

ofVec2f ofxUIMultiMovingGraph::getMaxAndMind()
{
    return ofVec2f(max, min);
}

void ofxUIMultiMovingGraph::setMaxAndMin(float _max, float _min)
{
    max = _max;
    min = _min;
}

// TODO: finish implementing (currently has no effect).
void ofxUIMultiMovingGraph::set_auto_bounds(bool _auto_bounds)
{
    auto_bounds = _auto_bounds;
}

//------------------------------------------------------

ofxUIMultiMovingGraph* ofxUIMultiMovingGraph::addToCanvas(ofxUICanvas* canvas, string _name, vector<ofVec3f> _buffer,
                                                          int _bufferSize, float _min, float _max,
                                                          float _h) {
    
    float globalGraphHeight = canvas->getGlobalGraphHeight();
    float w = canvas->getRect()->getWidth() - canvas->getWidgetSpacing() * 2;
    
    if(_h != globalGraphHeight)
    {
        _h = globalGraphHeight;
    }
    ofxUIMultiMovingGraph* widget = new ofxUIMultiMovingGraph(w, _h, _buffer, _bufferSize, _min, _max, _name);
    canvas->addWidgetPosition(widget, canvas->getWidgetPosition(), OFX_UI_ALIGN_LEFT);
    
    return widget;

}


