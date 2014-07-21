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
    init(inputDimension, vector<float>(inputDimension, -1.), vector<float>(inputDimension, 1.));
}

//--------------------------------------------------------------
void GVFHandler::init(int inputDimension, vector<float> _min_range, vector<float> _max_range)
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
    
    min_range = _min_range;
    max_range = _max_range;
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
        
        
        currentGesture->addObservation(observation_vector);
        
    }
    else if(getState() == ofxGVF::STATE_FOLLOWING)
    {
        vector<float> observation_vector(argc);
        for (int k=0; k < argc; k++) {
            observation_vector[k] = argv[k];
        }
        currentGesture->addObservation(observation_vector);
        
        // inference on the last observation
        infer(currentGesture->getLastObservation());
        
        WriteCsvData(&csv_recorder);
        
    }
}

//--------------------------------------------------------------
//--------------------------------------------------------------
// MARK: Recognition Information
//--------------------------------------------------------------
//--------------------------------------------------------------

//--------------------------------------------------------------
ofxGVFEstimation GVFHandler::getTemplateRecogInfo(int templateNumber) { // FIXME: Rename!
    
    // ???: Later transfer to an assert here. 
//    assert(templateNumber >= 0 && templateNumber < getOutcomes().estimations.size());
    
    if (getOutcomes().estimations.size() <= templateNumber) {
        ofxGVFEstimation estimation;
        return estimation; // blank
    }
    else
        return getOutcomes().estimations[templateNumber];
}

//--------------------------------------------------------------
ofxGVFEstimation GVFHandler::getRecogInfoOfMostProbable() // FIXME: Rename!
{
    int indexMostProbable = getMostProbableGestureIndex();
    
    if ((getState() == ofxGVF::STATE_FOLLOWING) && (getMostProbableGestureIndex() != -1)) {
        return getTemplateRecogInfo(indexMostProbable);
    }
    else {
        ofxGVFEstimation estimation;
        return estimation; // blank
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
    
    
    // FIXME: Input ranges need to be better adjusted. 
    int dim = getConfig().inputDimensions;
    currentGesture->setAutoAdjustRanges(false);
    currentGesture->setMinRange(vector<float>(dim, -100.));
    currentGesture->setMaxRange(vector<float>(dim, 100.));
    
    if (dim != 3 && dim != 2)
        currentGesture->setType(ofxGVFGesture::TEMPORAL);
    
    
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
