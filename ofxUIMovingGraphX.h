
/**********************************************************************************
 
 Copyright (C) 2012 Syed Reza Ali (www.syedrezaali.com)
 
 Created by Alejandro Van Zandt-Escobar on 04/11/2013, extension of ofxUIMovingGraph
 
 Edited to plot vector length n.
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 of the Software, and to permit persons to whom the Software is furnished to do
 so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 
 **********************************************************************************/

#include "ofxUIWidget.h"


#ifndef ofxgvf_ofxUIMovingGraphX_h
#define ofxgvf_ofxUIMovingGraphX_h

class ofxUIMovingGraphX : public ofxUIWidget
{
public:
    ofxUIMovingGraphX(float x, float y, float w, float h, vector<vector<float> >* _buffer, int _bufferSize, float _min, float _max, string _name) : ofxUIWidget()
    {
        rect = new ofxUIRectangle(x,y,w,h);
        init(w, h, _buffer, _bufferSize, _min, _max, _name);
    }
    
    ofxUIMovingGraphX(float w, float h, vector<vector<float> >* _buffer, int _bufferSize, float _min, float _max, string _name) : ofxUIWidget()
    {
        rect = new ofxUIRectangle(0,0,w,h);
        init(w, h, _buffer, _bufferSize, _min, _max, _name);
    }
    
    void init(float w, float h, vector<vector <float> >* _buffer, int _bufferSize, float _min, float _max, string _name)
    {
        name = string(_name);
        kind = OFX_UI_WIDGET_MOVINGGRAPH;
        
        paddedRect = new ofxUIRectangle(-padding, -padding, w+padding*2.0, h+padding*2.0);
        paddedRect->setParent(rect);
        
        draw_fill = true;
        draw_back = true;
        
        buffer = _buffer;					//the widget's value
        
        bufferSize = _bufferSize;
        max = _max;
        min = _min;
        scale = rect->getHeight()*.5;
        inc = rect->getWidth()/((float)bufferSize-1.0);
    }
    
    virtual void update()
    {
        drawFill();
    }
    
    virtual void drawFill()
    {
        // FIXME: draw background
        if (draw_back) {
            ofFill();
            ofSetColor(color_back);
            rect->draw();
        }
        if(draw_fill)
        {
            if (buffer->size() != 0) {
                for (int i = 0; i < (buffer->at(0)).size(); ++i)
                    drawLine(buffer, i);
            }
        }
    }
    
    void drawLine(vector<vector <float> >* _buffer, int track)
    {
        ofNoFill();
        if(draw_fill_highlight)
        {
            ofSetColor(color_fill_highlight);
        }
        else
        {
            ofSetColor(colors[track]);
        }
        ofPushMatrix();
        ofTranslate(rect->getX(), rect->getY()+scale, 0);
        ofSetLineWidth(1.5);
        ofBeginShape();
        int bufferLength = buffer->size();
        inc = rect->getWidth()/((float)bufferLength-1.0);
        for (int i = 0; i < buffer->size(); i++)
        {
            ofVertex(inc*(float)i, ofMap((buffer->at(i)).at(track), min, max, scale, -scale, true));
        }
        ofEndShape();
        ofSetLineWidth(1);
        ofPopMatrix();
    }
    
    
	void setParent(ofxUIWidget *_parent)
	{
		parent = _parent;
	}
	
    void addPoint(vector<float> _point)
    {
        buffer->push_back(_point);
        
        if( buffer->size() >= bufferSize )
        {
            buffer->erase(buffer->begin(), buffer->begin()+1);
        }
    }
    
    vector<vector<float> >* getBuffer()
    {
        return buffer;
    }
    
    void setBuffer(vector<vector<float> >* _buffer)
    {
        buffer = _buffer;
    }
    
    void setMax(float _max)
    {
        max = _max;
    }
    
    float getMax()
    {
        return max;
    }
    
    void setMin(float _min)
    {
        min = _min;
    }
    
    float getMin()
    {
        return min;
    }
    
    ofVec2f getMaxAndMind()
    {
        return ofVec2f(max, min);
    }
    
    void setMaxAndMin(float _max, float _min)
    {
        max = _max;
        min = _min;
    }
    
    void clear()
    {
        buffer->clear();
    }
    
protected:    //inherited: ofxUIRectangle *rect; ofxUIWidget *parent;
    vector<vector<float> >* buffer;
    float max, min, scale, inc;
    int bufferSize;
    ofColor colors[9] = {ofColor::blue, ofColor::turquoise, ofColor::green, ofColor::yellow,
        ofColor::orange, ofColor::red, ofColor::purple, ofColor::deepPink, ofColor::pink};
};

#endif
