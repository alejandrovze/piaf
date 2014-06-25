//
//  gvfPianoHandler.cpp
//  oFxGVFxPiano
//
//  Created by Alejandro Van Zandt-Escobar on 12/12/2013.
//
//

#include "gvfPianoHandler.h"

//--------------------------------------------------------------
gvfPianoHandler::gvfPianoHandler()
{
    
}

//--------------------------------------------------------------
gvfPianoHandler::~gvfPianoHandler()
{
    if (mygvf != NULL)
        delete mygvf;
}

//--------------------------------------------------------------
void gvfPianoHandler::setup(int inputDimension)
{
    currentGesture = new ofxGVFGesture(inputDimension);
    isPlaying = false;
    
    // --- GVF Setup ---
    
    // Configuration
    ofxGVFConfig        gC;
    gC.inputDimensions = inputDimension;
    gC.translate = true;
    gC.segmentation = true;
    
    // Parameters
    ofxGVFParameters    gP;
    gP.numberParticles = 2000;
    gP.tolerance = 0.2;
    gP.resamplingThreshold = 500;
    gP.distribution = 0.0f;
    gP.phaseVariance = 0.00001;
    gP.speedVariance = 0.00001;
    gP.scaleVariance = vector<float>(1, 0.00001);
    gP.rotationVariance = vector<float>(1, 0.00001);
    
    mygvf = new ofxGVF(gC, gP);
}

//--------------------------------------------------------------
//--------------------------------------------------------------
// MARK: Data Input
//--------------------------------------------------------------
//--------------------------------------------------------------


// Input vector, sent as array to data input method
//--------------------------------------------------------------
void gvfPianoHandler::gvf_data(std::vector<float> p)
{
    int s = p.size();
    float argvf[s];
    for (int i = 0; i < s; ++i)
        argvf[i] = p.at(i);
    
    gvf_data(s, argvf);
}

//--------------------------------------------------------------
void gvfPianoHandler::gvf_data(int argc, float *argv)
{
    if(mygvf->getState() == ofxGVF::STATE_CLEAR)
    {
        return;
    }
    if(argc == 0)
    {
        return;
    }
    if(mygvf->getState() == ofxGVF::STATE_LEARNING)
    {
        vector<float> observation_vector(argc);
        for (int k=0; k < argc; k++)
            observation_vector[k] = argv[k];
        
        
        // !!!
        currentGesture->setAutoAdjustRanges(false);
        currentGesture->setMin(0., 0., 0.);
        currentGesture->setMax(1000., 1000., 1000.);
        
        currentGesture->addObservation(observation_vector);
    }
    else if(mygvf->getState() == ofxGVF::STATE_FOLLOWING)
    {
        vector<float> observation_vector(argc);
        for (int k=0; k < argc; k++)
            observation_vector[k] = argv[k];
        
        currentGesture->addObservation(observation_vector);
        
        // inference on the last observation
        mygvf->infer(currentGesture->getLastObservation());
        
        UpdateRecogInfo();
        
    }
}

//--------------------------------------------------------------
//--------------------------------------------------------------
// MARK: Recognition Information
//--------------------------------------------------------------
//--------------------------------------------------------------

//--------------------------------------------------------------
void gvfPianoHandler::UpdateRecogInfo() {
    
    // Only if a gesture has been recognized.
    ofxGVFOutcomes gO = mygvf->getOutcomes();
    
    // Update Recognition Info for all Templates
    char temp[100];
    int  templates_count = mygvf->getNumberOfGestureTemplates();
    
    recogInfo.clear();
    
    for(int i = 0; i < templates_count; i++)
    {
        recognitionInfo info;
        
        info.probability = gO.allProbabilities[i];
        info.phase = gO.allPhases[i];
        info.speed = gO.allSpeeds[i];
        
        int numberOfScaleCoef = gO.allScales.size() / templates_count;
        info.scale = vector<float>(numberOfScaleCoef);
        for (int j= 0; j < numberOfScaleCoef; ++j) {
            info.scale[j] = gO.allScales[i * numberOfScaleCoef + j];
        }
        
        int numberOfRotationCoef = gO.allRotations.size() / templates_count;
        info.rotation = vector<float>(numberOfRotationCoef);
        for (int j = 0; j < numberOfRotationCoef; ++j) {
            info.rotation[j] = gO.allRotations[i * numberOfRotationCoef + j];
        }
        
        recogInfo.push_back(info);
    }
}

//--------------------------------------------------------------
recognitionInfo gvfPianoHandler::getTemplateRecogInfo(int templateNumber) {
    
    if (recogInfo.size() <= templateNumber) {
        recognitionInfo blankRecogInfo = {0., 0., 0.};
        return blankRecogInfo;
    }
    else
        return recogInfo.at(templateNumber);
}

//--------------------------------------------------------------
recognitionInfo gvfPianoHandler::getRecogInfoOfMostProbable()
{
    int indexMostProbable = mygvf->getMostProbableGestureIndex();
    
    if (mygvf->getState() == ofxGVF::STATE_FOLLOWING) {
        if (indexMostProbable == -1) { // Need this because sometimes goes out of bounds.
            recognitionInfo blankRecogInfo = {0., 0., 0.};
            return blankRecogInfo;
        }
        else
            return getTemplateRecogInfo(indexMostProbable);
    }
    else {
        recognitionInfo blankRecogInfo = {0., 0., 0.};
        return blankRecogInfo;
    }
}




//--------------------------------------------------------------
//--------------------------------------------------------------
// MARK: Gets & Sets
//--------------------------------------------------------------
//--------------------------------------------------------------

//--------------------------------------------------------------
ofxGVF* gvfPianoHandler::getGVF() {
    return mygvf;
}

//--------------------------------------------------------------
ofxGVFGesture* gvfPianoHandler::getCurrentGesture(){
    return currentGesture;
}

//--------------------------------------------------------------
void gvfPianoHandler::startGesture() {
    
    currentGesture->clear();
    
    if(mygvf->getState() == ofxGVF::STATE_FOLLOWING)
        mygvf->spreadParticles();
    
}

//--------------------------------------------------------------
void gvfPianoHandler::endGesture() {
    
    if (mygvf->getState() == ofxGVF::STATE_LEARNING)
        mygvf->addGestureTemplate(*currentGesture);
    
}

//--------------------------------------------------------------
void gvfPianoHandler::setState(ofxGVF::ofxGVFState state) {
    
    int currentState = mygvf->getState();
    
    switch (state)
    {
        case ofxGVF::STATE_LEARNING:
            if (currentState != ofxGVF::STATE_LEARNING && !isPlaying) {
                mygvf->setState(ofxGVF::STATE_LEARNING);
            }
            break;
        case ofxGVF::STATE_FOLLOWING:
            if (currentState != ofxGVF::STATE_FOLLOWING && !isPlaying) {
                
                mygvf->learn();
                
                if (mygvf->getNumberOfGestureTemplates() != 0)
                    mygvf->setState(ofxGVF::STATE_FOLLOWING);
                
            }
            break;
        case ofxGVF::STATE_CLEAR:
            if (!isPlaying) { // Don't clear if playing.
                mygvf->clear();
            }
            break;
    }
}

//--------------------------------------------------------------
bool gvfPianoHandler::getIsPlaying() {
    return isPlaying;
}

//--------------------------------------------------------------
bool gvfPianoHandler::toggleIsPlaying() {
    
    if (mygvf->getState() != ofxGVF::STATE_CLEAR) { // Don't toggle when cleared
        
        isPlaying = !isPlaying;
        
        if (!isPlaying)
            endGesture();
        
        if (isPlaying)
            startGesture();
    }
    
    return isPlaying;
}
