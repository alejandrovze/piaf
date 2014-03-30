//
//  gvfPianoApp.h
//  oFxGVFxPiano
//
//  Created by Alejandro Van Zandt-Escobar on 26/11/2013.
//
//

#ifndef __oFxGVFxPiano__gvfPianoApp__
#define __oFxGVFxPiano__gvfPianoApp__

#include <iostream>

#include "gvfPianoInputs.h"
#include "gvfPianoHandler.h"

typedef struct {
    vector<vector<float> > data;
    int id;
    string name;
    // Maybe add more "metadata"
}  templateGesture;

class gvfPianoApp {
    
public:
    
    gvfPianoApp();
    
    void setup();
    void update();
    
    void setState(ofxGVF::ofxGVFState state);
    void setFollowing(bool _isFollowing);
    
    void setParameters(vector<float>); //!!!
    
    // methods used to change gvf parameters
    void setSmoothingCoefficient(float smoothingCoeficient);
    void setResamplingThreshold(int resamplingThreshold);
    void setAdaptSpeed(ofxGVF::ofxGVFVarianceCoefficents varianceCoefficients);
    void setNumberOfParticles(int numParticles);
    
    void saveGestures(string filename);
    void loadGestures(string filename);
    
    ofxGVF::ofxGVFState getState();
    bool getFollowing();
    vector<float> getParameters();
    vector<float> getInputData();
    vector<gvfInputInfo> getInputsInfo();
    
    vector<vector<float> >* getGestureData();
    
    // Get templates.
    templateGesture getTemplateGesture(int template_id); //TODO
    int getTemplateCount();
    vector<vector<float> > getTemplateData(int template_id);
    int getTemplatesLearned(); // Updated once new gesture is finished being recorded. 
    
    int getIndexMostProbable();
    recognitionInfo getTemplateInfo(int template_id); // Gets recognition info
    recognitionInfo getInfoOfMostProbable();

    
private:
    
    gvfPianoHandler* gvfh;
    ofxGVF* gvf;
    
    gvfPianoInputs inputHandler;
    
    std::vector<float> inputData;
    int inputVectorSize;
    
    vector<vector<float> > gesture_data; // Holds history of data for current gesture;
    
    void gestureStart();
    void gestureStop();
    void gestureInput(std::vector<float> inputData);
    
    bool isFollowing = false; //!!!: it might be less ambiguous to name "isPlaying"
    
    int mostProbableTemplateID;
    int templates_learned;
    
};

#endif /* defined(__oFxGVFxPiano__gvfPianoApp__) */
