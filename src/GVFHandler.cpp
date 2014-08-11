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
    
    // Parameters for tracking MOCAP
    ofxGVFParameters mocap_params;
    mocap_params.numberParticles = 2000;
    mocap_params.tolerance = 0.2;
    mocap_params.resamplingThreshold = 500;
    mocap_params.distribution = 0.0;
    mocap_params.phaseVariance = 0.000001;
    mocap_params.speedVariance = 0.1;
    mocap_params.scaleVariance = vector<float>(3, 0.1);
    mocap_params.scaleVariance[1] = 0.0001;
    mocap_params.rotationVariance = vector<float>(3, 0.00001);
    
    
    setup(gC, mocap_params);
    
    min_range = _min_range;
    max_range = _max_range;
    
    csv_path = ofToDataPath("dump/gvf-dump-");
    n_file = 0;
}

//--------------------------------------------------------------
void GVFHandler::set_csv_path(string path) {
    
    csv_path = path;
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
    
    // FIXME: Quick "dirty" scaling.
    
//    cout << "OBSERVATION" << endl;
    vector<float> observation_vector(argc);
    for (int k=0; k < argc; k++) {
        observation_vector[k] = (argv[k] - currentGesture->getMinRange()[k]) / (currentGesture->getMaxRange()[k] - currentGesture->getMinRange()[k]);
        
//        cout << observation_vector[k] << " ";
    }
//    cout << endl;
    
    
    if(getState() == ofxGVF::STATE_LEARNING)
    {
        currentGesture->addObservation(observation_vector);
        
    }
    else if(getState() == ofxGVF::STATE_FOLLOWING)
    {

        currentGesture->addObservation(observation_vector);
        
        // inference on the last observation
        infer(currentGesture->getLastObservation());
        
        WriteCsvDataRow();
    }
}

//--------------------------------------------------------------
//--------------------------------------------------------------
// MARK: Recognition Information
//--------------------------------------------------------------
//--------------------------------------------------------------


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
    currentGesture->setMinRange(min_range);
    currentGesture->setMaxRange(max_range);
    
    if (dim != 3 && dim != 2)
        currentGesture->setType(ofxGVFGesture::TEMPORAL);
    
    
    if(getState() == ofxGVF::STATE_FOLLOWING) {
        SetupCsvData();
        
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
            csv_recorder.saveFile(csv_path + ofToString(n_file++) + ".csv");
            WriteCsvTemplates();
            WriteCsvSettings();
            break;
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

//--------------------------------------------------------------
//--------------------------------------------------------------
void GVFHandler::SetupCsvData() {
    
    csv_recorder.clear();
    csv_recorder.createFile(csv_path + ofToString(n_file) + ".csv");
    csv_recorder.fileSeparator = ' ';
    
    int n_templates = getNumberOfGestureTemplates();
    int scale_dim = getRecogInfoOfMostProbable().scale.size();
    int rotation_dim = getRecogInfoOfMostProbable().rotation.size();
    int info_length = 1 + 1 + 1 + 1 + scale_dim + rotation_dim; // id + prob + phase + speed + rot + scale;
    
    csv_recorder.setInt(0, 0, n_templates);
    csv_recorder.setInt(0, 1, getConfig().inputDimensions);
    csv_recorder.setInt(0, 2, info_length);
    csv_recorder.setInt(0, 3, rotation_dim);
    csv_recorder.setInt(0, 4, scale_dim);
    
}
//--------------------------------------------------------------
//--------------------------------------------------------------
void GVFHandler::WriteCsvDataRow() {
    
    int row = csv_recorder.numRows;
    
    int column = 0;
    
    int input_dim = currentGesture->getNumberDimensions();
    
    csv_recorder.setInt(row, column++, row);
    
    // Write input data.
    for (int i = 0; i < input_dim; ++i)
        csv_recorder.setFloat(row, column + i, currentGesture->getLastObservation()[i]);
    
    column += input_dim;
    
    // Write Most Probable Index
    csv_recorder.setInt(row, column++, getMostProbableGestureIndex());
    
    // Write Recognition Information
    for (int i = 0; i < getNumberOfGestureTemplates(); ++i) {
        
        ofxGVFEstimation estimation = getOutcomes().estimations[i];
        
        // Write Template Number
        csv_recorder.setInt(row, column++, i);
        
        // Write Probability
        csv_recorder.setFloat(row, column++, estimation.probability);
        
        // Write Phase
        csv_recorder.setFloat(row, column++, estimation.phase);
        
        // Write Speed
        csv_recorder.setFloat(row, column++, estimation.speed);
        
        // Write Rotation
        for (int j = 0; j < estimation.rotation.size(); ++j)
            csv_recorder.setFloat(row, column++, estimation.rotation[j]);
        
        // Write Scale
        for (int j = 0; j < estimation.scale.size(); ++j)
            csv_recorder.setFloat(row, column++, estimation.scale[j]);
            
    }
    
}

//--------------------------------------------------------------
void GVFHandler::WriteCsvSettings() {
    
    wng::ofxCsv csv;
    
    string file_name = csv_path + ofToString(n_file) + "-settings.csv";
    
    csv.clear();
    csv.createFile(file_name);
    csv.fileSeparator = ' ';
    
    ofxGVFParameters params = getParameters();
    
    int row = 0;
    
    csv.setInt(row++, 0, params.numberParticles);
    csv.setFloat(row++, 0, params.tolerance);
    csv.setInt(row++, 0, params.resamplingThreshold);
    csv.setFloat(row++, 0, params.distribution);
    csv.setFloat(row++, 0, params.phaseVariance);
    csv.setFloat(row++, 0, params.speedVariance);
    
    for(int i = 0; i < params.scaleVariance.size(); ++i) {
        csv.setFloat(row, i, params.scaleVariance[i]);
    }
    ++row;
    
    for(int i = 0; i < params.rotationVariance.size(); ++i) {
        csv.setFloat(row, i, params.rotationVariance[i]);
    }
    ++row;
    
    
    csv.setFloat(row++, 0, params.phaseInitialSpreading);
    
    
    csv.setFloat(row++, 0, params.speedInitialSpreading);
    
    for(int i = 0; i < params.scaleInitialSpreading.size(); ++i) {
        csv.setFloat(row, i, params.scaleInitialSpreading[i]);
    }
    ++row;
    
    for(int i = 0; i < params.rotationInitialSpreading.size(); ++i) {
        csv.setFloat(row, i, params.rotationInitialSpreading[i]);
    }

    csv.saveFile(file_name);
}


//--------------------------------------------------------------
void GVFHandler::WriteCsvTemplates() {
    
    wng::ofxCsv csv;
    
    string file_name = csv_path + ofToString(n_file) + "-templates.csv";
    
    csv.clear();
    csv.createFile(file_name);
    csv.fileSeparator = ' ';
    
    vector<ofxGVFGesture> templates = getAllGestureTemplates();
    
    int input_dim = templates[0].getNumberDimensions();
    int n_templates = templates.size();
    

    csv.setInt(0, 0, input_dim); // n templates
    csv.setInt(0, 1, n_templates); // input dimensions
    
    int row = 1;
    
    for (int i = 0; i < templates.size(); i++) {
        
        int template_length = templates[i].getTemplateLength();
        
        vector<vector<float> >& data = templates[i].getTemplatesNormal()[0];
        
        csv.setInt(0, i + 2, template_length); // Template Length
        
        for (int j = 0; j < template_length; ++j) {
            
            for (int k = 0; k < input_dim; ++k) {
                csv.setFloat(row, k, data[j][k]);
            }
            
            ++row;
        }
        
    }
    
    csv.saveFile(file_name);
}
