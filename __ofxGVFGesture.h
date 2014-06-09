//
//  ofxGVFGesture.h
//  ofxGVF-Example
//
//  Created by gameover on 10/12/13.
//
//

#ifndef _H_OFXGVF_GESTURE
#define _H_OFXGVF_GESTURE

#include "ofxGVFTypes.h"


class ofxGVFGesture {
    
    
    
private:
    
    string name;
    int inputDimensions;
    ofxGVFGestureType type;
    vector<float> observationRangeMax;
    vector<float> observationRangeMin;
    bool bAutoAdjustNormalRange;
    bool bIsRangeMaxSet;
    bool bIsRangeMinSet;
    //    int id;
    
    vector<float> templateInitialRaw;
    vector<float> templateInitialNormal;
    
    vector< vector< vector<float> > > templatesRaw;
    vector< vector< vector<float> > > templatesNormal;
    
    

    
public:
    
    enum ofxGVFGestureType{
        GEOMETRIC,
        TEMPORAL
    };
    
    ofxGVFGesture(){
        inputDimensions = 2; // default to 2D
        type = GEOMETRIC; // default to a geometric shape
        bAutoAdjustNormalRange = false;
        clear();
    };
    
    ~ofxGVFGesture(){
        clear();
    };
    
    void setName(string name){
        this->name = name;
    }
    
    string getName(){
        return name;
    }
    
    void setType(ofxGVFGestureType type){
        this->type = type;
        normalise();
    }
    
    ofxGVFGestureType getType(){
        return type;
    }
    
    void setNumberDimensions(int dimensions){
        assert(dimensions > 0);
        inputDimensions = dimensions;
    }
    
    void setAutoAdjustRanges(bool b){
        if(b) bIsRangeMinSet = bIsRangeMaxSet = false;
        bAutoAdjustNormalRange = b;
    }
   
    
    void setMax(float x, float y){
        assert(inputDimensions == 2);
        vector<float> r(2);
        r[0] = x; r[1] = y;
        setMaxRange(r);
    }
    
    void setMin(float x, float y){
        assert(inputDimensions == 2);
        vector<float> r(2);
        r[0] = x; r[1] = y;
        setMinRange(r);
    }
    
    void setMax(float x, float y, float z){
        assert(inputDimensions == 3);
        vector<float> r(3);
        r[0] = x; r[1] = y; r[2] = z;
        setMaxRange(r);
    }
    
    void setMin(float x, float y, float z){
        assert(inputDimensions == 3);
        vector<float> r(3);
        r[0] = x; r[1] = y; r[2] = z;
        setMinRange(r);
    }
    
    void setMaxRange(vector<float> observationRangeMax){
        this->observationRangeMax = observationRangeMax;
        bIsRangeMaxSet = true;
        normalise();
    }
    
    void setMinRange(vector<float> observationRangeMin){
        this->observationRangeMin = observationRangeMin;
        bIsRangeMinSet = true;
        normalise();
    }
    
    vector<float>& getMaxRange(){
        return observationRangeMax;
    }
    
    vector<float>& getMinRange(){
        return observationRangeMin;
    }
    
    void autoAdjustMinMax(vector<float> & observation){
        if(observationRangeMax.size()  < inputDimensions){
            observationRangeMax.assign(inputDimensions, -INFINITY);
            observationRangeMin.assign(inputDimensions,  INFINITY);
        }
        for(int i = 0; i < inputDimensions; i++){
            observationRangeMax[i] = MAX(observationRangeMax[i], observation[i]);
            observationRangeMin[i] = MIN(observationRangeMin[i], observation[i]);
        }
    }
    
    void addObservationRaw(vector<float> observation, int templateIndex = 0){
        
        // check we have a valid templateIndex and correct number of input dimensions
        assert(templateIndex <= templatesRaw.size());
        assert(observation.size() == inputDimensions);
        assert(bAutoAdjustNormalRange || (bIsRangeMaxSet && bIsRangeMinSet));
        
        // if the template index is same as the number of temlates make a new template
        if(templateIndex == templatesRaw.size()){ // make a new template
            
            // reserve space in raw and normal template storage
            templatesRaw.resize(templatesRaw.size() + 1);
            templatesNormal.resize(templatesNormal.size() + 1);
            
        }

        if(templatesRaw[templateIndex].size() == 0){
            templateInitialRaw = templateInitialNormal = observation;
        //    cout << "INIT " << observation[0] << " " << observation[1] << endl;
        }

        for(int j = 0; j < observation.size(); j++){
            observation[j] = observation[j] - templateInitialRaw[j];
        }
        //            cout << "THEN " << observation.size() << " | " << observation[0] << " " << observation[1] << " " << templateInitialRaw[0] << " " << templateInitialRaw[1] << endl;
        
        // store the raw observation
        templatesRaw[templateIndex].push_back(observation);
        
        // if set let's auto size the range for normalising
        if(bAutoAdjustNormalRange) autoAdjustMinMax(observation);
        
        normalise();
        
    }
    
    void normalise(){
        

        
        // reserve space for raw and normal meshes
        representationsNormal.resize(templatesRaw.size() + 1);
        
        templatesNormal.resize(templatesRaw.size());
        
        for(int t = 0; t < templatesRaw.size(); t++){
            
            if(type == GEOMETRIC){
                
                // for GEOMETRIC representations let's use a single mesh with n-Dimensions
                
                representationsNormal[t].resize(1);
                representationsNormal[t][0].setMode(OF_PRIMITIVE_LINE_STRIP);
                
            }else{
                
                // for TEMPORAL representations let's use a mesh for EACH of the n-Dimensions
                
                representationsNormal[t].resize(inputDimensions);
                for(int i = 0; i < inputDimensions; i++){
                    representationsNormal[t][i].setMode(OF_PRIMITIVE_LINE_STRIP);
                }
            }
            
            for(int m = 0; m < representationsNormal[t].size(); m++){
                representationsNormal[t][m].clear();
            }

            
            templatesNormal[t].resize(templatesRaw[t].size());
            
            for(int o = 0; o < templatesRaw[t].size(); o++){
  
                templatesNormal[t][o].resize(inputDimensions);
                
                for(int d = 0; d < inputDimensions; d++){
                    
                    //cout << d << " " << templatesNormal[t][o][d] << " " << templatesRaw[t][o][d] / ABS(observationRangeMax[d] - observationRangeMin[d]) << endl;
                    
                    templatesNormal[t][o][d] = templatesRaw[t][o][d] / (observationRangeMax[d] - observationRangeMin[d]);
                    templateInitialNormal[d] = templateInitialRaw[d] / (observationRangeMax[d] - observationRangeMin[d]);

            }
        }
        
        bIsRangeMinSet = bIsRangeMaxSet = true;
        
    }

    void setTemplateRaw(vector< vector<float> > & observations, int templateIndex = 0){
        for(int i = 0; i < observations.size(); i++){
            addObservationRaw(observations[i], templateIndex);
        }
    }
    
    vector< vector<float> > & getTemplateRaw(int templateIndex = 0){
        assert(templateIndex < templatesRaw.size());
        return templatesRaw[templateIndex];
    }

    vector< vector<float> > & getTemplateNormal(int templateIndex = 0){
        assert(templateIndex < templatesNormal.size());
        return templatesNormal[templateIndex];
    }
    
    int getNumberOfTemplates(){
        return templatesRaw.size();
    }
    
    int getTemplateLength(int templateIndex = 0){
        return templatesRaw[templateIndex].size();
    }
    
    vector<float>& getLastRawObservation(int templateIndex = 0){
        return templatesRaw[templateIndex][templatesRaw[templateIndex].size() - 1];
    }
    
    vector<float>& getLastNormalObservation(int templateIndex = 0){
        return templatesNormal[templateIndex][templatesNormal[templateIndex].size() - 1];
    }
    
    vector< vector< vector<float> > >& getTemplatesRaw(){
        return templatesRaw;
    }
    
    vector< vector< vector<float> > >& getTemplatesNormal(){
        return templatesNormal;
    }
    
    vector<float>& getInitialObservationRaw(){
        return templateInitialRaw;
    }
    
    vector<float>& getInitialObservationNormal(){
        return templateInitialNormal;
    }
    
    void deleteTemplate(int templateIndex = 0){
        assert(templateIndex < templatesRaw.size());
        templatesRaw[templateIndex].clear();
        templatesNormal[templateIndex].clear();

    }
    
    void clear(){
        templatesRaw.clear();
        templatesNormal.clear();

        
        bIsRangeMinSet = false;
        bIsRangeMaxSet = false;
        observationRangeMax.assign(inputDimensions, -INFINITY);
        observationRangeMin.assign(inputDimensions,  INFINITY);
    }

      
};


#endif