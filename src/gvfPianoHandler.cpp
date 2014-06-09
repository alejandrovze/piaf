//
//  gvfPianoHandler.cpp
//  ofgvfVisualiser
//
//  Created by Thomas Rushmore on 21/06/2013.
//  Modified by Igor Correa
//  Modified by Alejandro Van Zandt-Escobar

//
//  gvfPianoHandler.cpp
//  oFxGVFxPiano
//
//  Created by Alejandro Van Zandt-Escobar on 12/12/2013.
//
//

#include "gvfPianoHandler.h"


// Rename "state" to "mode"

// recognition info in the GVF ==> name variation info
//--> one funcciton, get info, arguments to get all or get specific (and call index most prob to get info of most prob)
// --> can eliminate "get most probable probability"

// elegant way of getting number of templates learned vs number of templates started? in GVF
// removing template (in GVF)
// replacing template (clear and re-input data). 
// "data input", fed depending on the current state (in handler or GVF?)
// ??? restart? restart_d; restart_l; --> in GVF?
// transposition done in the GVF

// scale as vector in the GVF

gvfPianoHandler::gvfPianoHandler(int _inputDimension)
{
    setup(_inputDimension);   
}

gvfPianoHandler::~gvfPianoHandler()
{
    if (mygvf!=NULL)
        delete mygvf;
}

// Set state to learn...add template, fill template, etc.
void gvfPianoHandler::setup(int _inputDimension)
{
    // INPUT DIMENSION == parameter
    
    inputDimension = _inputDimension;
    stateDimension = 2 + inputDimension;             //!!! TODO state dimension is hard coded here
    
    // Configuration
    gC.inputDimensions = inputDimension;
    gC.translate = true;
    gC.segmentation = true;
    
    // Parameters
    gP.numberParticles = 2000;
    gP.tolerance = 0.2f;
    gP.resamplingThreshold = 500;
    gP.distribution = 0.0f;
    gP.phaseVariance = 0.00001;
    gP.speedVariance = 0.00001;
    gP.scaleVariance = 0.00001;
    gP.rotationVariance = 0.00001;
    
    mygvf = new ofxGVF(gC, gP);
    currentGesture = new ofxGVFGesture();
}


// Set state to learn...add template, fill template, etc. 
void gvfPianoHandler::gvf_learn()
{
    mygvf->setState(ofxGVF::STATE_LEARNING); //!!!: ofxGVF
}

void gvfPianoHandler::gvf_follow()
{
    mygvf->setState(ofxGVF::STATE_FOLLOWING);  //!!!: ofxGVF
}

void gvfPianoHandler::gvf_clear()
{
    mygvf->clear();
}


// Input vector, sent as array to data input method
void gvfPianoHandler::gvf_data(std::vector<float> p)
{
    int s = p.size();
    float argvf[s];
    for (int i = 0; i < s; ++i)
        argvf[i] = p.at(i);
    
    gvf_data(s, argvf);
}

// !!!: This works, but maybe needs cleaning up / simplication to interface with gvf
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
        for (int k=0; k<argc; k++)
            observation_vector[k] = argv[k];
        
        currentGesture->addObservation(observation_vector);
        
    }
    else if(mygvf->getState() == ofxGVF::STATE_FOLLOWING)
    {
        vector<float> observation_vector(argc);
        for (int k=0; k<argc; k++)
            observation_vector[k] = argv[k];
        
        currentGesture->addObservation(observation_vector);
        
        // inference on the last observation
        mygvf->infer(currentGesture->getLastObservation());
        
        // output recognition
        gO = mygvf->getOutcomes();
        
        
        char temp[100];
        int  templates_count = mygvf->getNumberOfGestureTemplates();
        
        recogInfo.clear();
        
        for(int i = 0; i < templates_count; i++)
        {
            recognitionInfo info;

            info.probability = gO.allProbabilities[i];
            info.phase = gO.allPhases[i];
            info.speed = gO.allSpeeds[i];
            
            int numberOfScaleCoef = gO.allScales.size()/templates_count;
            info.scale = vector<float>(numberOfScaleCoef);
            for (int j= 0; j < numberOfScaleCoef; ++j) {
                info.scale[j] = gO.allScales[i*numberOfScaleCoef+j];
            }

            // TODO rotation?
            
            recogInfo.push_back(info);
        }
        
    }
}


void gvfPianoHandler::gvf_restart()
{
    currentGesture->clear();
    
    if(mygvf->getState() == ofxGVF::STATE_FOLLOWING)
        mygvf->spreadParticles();
}



// !!!: Maybe messy & instead should come straight from GVF
std::vector<recognitionInfo> gvfPianoHandler::getRecogInfo()
{
    return recogInfo;
}


recognitionInfo gvfPianoHandler::getTemplateRecogInfo(int templateNumber) {
    
    if (getRecogInfo().size() <= templateNumber) {
        recognitionInfo blankRecogInfo = {0., 0., 0., 0.};
        return blankRecogInfo;
    }
    else
        return getRecogInfo().at(templateNumber);
}

// FIXME: Seems to be issue in order in which we get recognition info and update it,
// causing calls out of bounds.
// FIXME: Another issue is that it seems that the phase sometimes returns a value which
// is not a number, which causes the system to crash when trying to display that in the
// interface. We need to figure out why this happens. It seems that phase approaches 1
// and then instead of reaching 1 crashes. (might this have to do with segmentation and/or
// resetting when the end of a gesture is reached?)
recognitionInfo gvfPianoHandler::getRecogInfoOfMostProbable()
{
    int indexMostProbable = mygvf->getMostProbableGestureIndex();
    
    if (mygvf->getState() == ofxGVF::STATE_FOLLOWING) {
        if (indexMostProbable == -1) { // Need this because sometimes goes out of bounds.
            recognitionInfo blankRecogInfo = {0., 0., 0., 0.};
            return blankRecogInfo;
        }
        else
            return getTemplateRecogInfo(indexMostProbable);
    }
    else {
        recognitionInfo blankRecogInfo = {0., 0., 0., 0.};
        return blankRecogInfo;
    }
}

ofxGVF* gvfPianoHandler::getGVF() {
    return mygvf;
}

ofxGVFGesture* gvfPianoHandler::getCurrentGesture(){
    return currentGesture;
}

//
//// TODO: Implement
//void gvfPianoHandler::removeTemplateGesture(int templateNumber) {
//    
//}


void gvfPianoHandler::saveGestures(string fn)
{
    mygvf->saveTemplates(fn);
}

void gvfPianoHandler::loadGestures(string fn)
{
    mygvf->loadTemplates(fn);
}
