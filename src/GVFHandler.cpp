//
//  GVFHandler.cpp
//  piaf
//
//  Created by Alejandro Van Zandt-Escobar on 10/07/2014.
//
//

#include "GVFHandler.h"

//--------------------------------------------------------------
void GVFHandler::init(int inputDimension)
{
    currentGesture = new ofxGVFGesture(inputDimension);
    isPlaying = false;
    
    // --- GVF Setup ---
    
    // Configuration
    ofxGVFConfig        gC;
    gC.inputDimensions = inputDimension;
    gC.translate = true;
    gC.segmentation = true;
    
    setup(gC);
}

//--------------------------------------------------------------
//--------------------------------------------------------------
// MARK: Data Input
//--------------------------------------------------------------
//--------------------------------------------------------------


// Input vector, sent as array to data input method
//--------------------------------------------------------------
void GVFHandler::gvf_data(std::vector<float> p)
{
    int s = p.size();
    float argvf[s];
    for (int i = 0; i < s; ++i)
        argvf[i] = p.at(i);
    
    gvf_data(s, argvf);
}

//--------------------------------------------------------------
void GVFHandler::gvf_data(int argc, float *argv)
{
    if(getState() == ofxGVF::STATE_CLEAR)
    {
        return;
    }
    if(argc == 0)
    {
        return;
    }
    if(getState() == ofxGVF::STATE_LEARNING)
    {
        vector<float> observation_vector(argc);
        for (int k=0; k < argc; k++)
            observation_vector[k] = argv[k];
        
        // FIXME: Range Adjustment
        currentGesture->setAutoAdjustRanges(false);
        currentGesture->setMinRange(vector<float>(3, -100.));
        currentGesture->setMaxRange(vector<float>(3, 100.));
        
        currentGesture->addObservation(observation_vector);
        
    }
    else if(getState() == ofxGVF::STATE_FOLLOWING)
    {
        vector<float> observation_vector(argc);
        for (int k=0; k < argc; k++) {
            observation_vector[k] = argv[k];
        }
        
        // FIXME: Range Adjustment
        currentGesture->setAutoAdjustRanges(false);
        currentGesture->setMinRange(vector<float>(3, -100.));
        currentGesture->setMaxRange(vector<float>(3, 100.));
        
        currentGesture->addObservation(observation_vector);
        
        // inference on the last observation
        infer(currentGesture->getLastObservation());
        
        UpdateRecogInfo();
        
        WriteCsvData(&csv_recorder);
        
    }
}

//--------------------------------------------------------------
//--------------------------------------------------------------
// MARK: Recognition Information
//--------------------------------------------------------------
//--------------------------------------------------------------

//--------------------------------------------------------------
void GVFHandler::UpdateRecogInfo() {
    
    // Only if a gesture has been recognized.
    ofxGVFOutcomes outcomes = getOutcomes();
    
    // Update Recognition Info for all Templates
    char temp[100];
    int  templates_count = getNumberOfGestureTemplates();
    
    recogInfo.clear();
    
    for(int i = 0; i < templates_count; i++)
    {
        Estimation info;
        
        info.probability = outcomes.allProbabilities[i];
        info.phase = outcomes.allPhases[i];
        info.speed = outcomes.allSpeeds[i];
        
        int numberOfScaleCoef = outcomes.allScales.size() / templates_count;
        info.scale = vector<float>(numberOfScaleCoef);
        for (int j= 0; j < numberOfScaleCoef; ++j) {
            info.scale[j] = outcomes.allScales[i * numberOfScaleCoef + j];
        }
        
        int numberOfRotationCoef = outcomes.allRotations.size() / templates_count;
        info.rotation = vector<float>(numberOfRotationCoef);
        for (int j = 0; j < numberOfRotationCoef; ++j) {
            info.rotation[j] = outcomes.allRotations[i * numberOfRotationCoef + j];
        }
        
        recogInfo.push_back(info);
    }
}

//--------------------------------------------------------------
Estimation GVFHandler::getTemplateRecogInfo(int templateNumber) {
    
    if (recogInfo.size() <= templateNumber) {
        Estimation blankRecogInfo = {0., 0., 0.};
        return blankRecogInfo;
    }
    else
        return recogInfo.at(templateNumber);
}

//--------------------------------------------------------------
Estimation GVFHandler::getRecogInfoOfMostProbable()
{
    int indexMostProbable = getMostProbableGestureIndex();
    
    if (getState() == ofxGVF::STATE_FOLLOWING) {
        if (indexMostProbable == -1) { // Need this because sometimes goes out of bounds.
            Estimation blankRecogInfo = {0., 0., 0.};
            return blankRecogInfo;
        }
        else
            return getTemplateRecogInfo(indexMostProbable);
    }
    else {
        Estimation blankRecogInfo = {0., 0., 0.};
        return blankRecogInfo;
    }
}

//--------------------------------------------------------------
//--------------------------------------------------------------
// MARK: Gets & Sets
//--------------------------------------------------------------
//--------------------------------------------------------------

//--------------------------------------------------------------
ofxGVFGesture* GVFHandler::getCurrentGesture(){
    return currentGesture;
}

//--------------------------------------------------------------
void GVFHandler::startGesture() {
    
    currentGesture->clear();
    
    if(getState() == ofxGVF::STATE_FOLLOWING) {
        csv_recorder.clear();
        csv_recorder.createFile(ofToDataPath("new_file.csv"));
        csv_recorder.fileSeparator = ' ';
        
        
        spreadParticles();
    }
    
}

//--------------------------------------------------------------
void GVFHandler::endGesture() {
    
    switch (getState())
    {
        case ofxGVF::STATE_LEARNING:
            addGestureTemplate(*currentGesture);
            break;
        case ofxGVF::STATE_FOLLOWING:
            csv_recorder.saveFile(ofToDataPath("new_file.csv"));
        default:
            break;
    }
    
    
}

// PROBLEMS WITH INHERITANCE
// Wrapper function, but then calls set_state in GVF. 
//--------------------------------------------------------------
void GVFHandler::set_state(ofxGVF::ofxGVFState _state) {
    
    switch (_state)
    {
        case ofxGVF::STATE_LEARNING:
            if (getState() != ofxGVF::STATE_LEARNING && !isPlaying) {
                setState(ofxGVF::STATE_LEARNING);
            }
            break;
        case ofxGVF::STATE_FOLLOWING:
            if (getState() != ofxGVF::STATE_FOLLOWING && !isPlaying) {
                
                learn();
                
                if (getNumberOfGestureTemplates() != 0)
                    setState(ofxGVF::STATE_FOLLOWING);
                
            }
            break;
        case ofxGVF::STATE_CLEAR:
            if (!isPlaying) { // Don't clear if playing.
                clear();
            }
            break;
    }
}

//--------------------------------------------------------------
bool GVFHandler::getIsPlaying() {
    return isPlaying;
}

//--------------------------------------------------------------
bool GVFHandler::toggleIsPlaying() {
    
    if (getState() != ofxGVF::STATE_CLEAR) { // Don't toggle when cleared
        
        isPlaying = !isPlaying;
        
        if (!isPlaying)
            endGesture();
        
        if (isPlaying)
            startGesture();
    }
    
    return isPlaying;
}


//--------------------------------------------------------------
//--------------------------------------------------------------
// MARK: Data Output
//--------------------------------------------------------------
//--------------------------------------------------------------
void GVFHandler::WriteCsvData(wng::ofxCsv* _csv_recorder) {
    
    int row = _csv_recorder->numRows;
    
    int input_dim = currentGesture->getNumberDimensions();
    
    _csv_recorder->setInt(row, 0, row);
    
    // Write input data.
    for (int i = 0; i < currentGesture->getNumberDimensions(); ++i)
        _csv_recorder->setFloat(row, i + 1, currentGesture->getLastObservation()[i]);
    
    // Write Most Probable Index
    _csv_recorder->setInt(row, input_dim + 1, getMostProbableGestureIndex());
    
    // Write Recognition Information
    for (int i = 0; i < getNumberOfGestureTemplates(); ++i) {
        // TODO: (needs ofxGVF output first).
    }
    
}
