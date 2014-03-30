//
//  gvfKinectHandler.cpp
//  mvf
//
//  Created by Alejandro Van Zandt-Escobar on 28/03/2014.
//
//

#include "gvfKinectHandler.h"

gvfKinectHandler::gvfKinectHandler()
{
  // initialisation of parameters. Functions are provided to change most of them at runtime
  
  // PARAMETERS
  ofxGVF::ofxGVFParameters parameters;
  
  parameters.inputDimensions = 3; // MARK: input dimensions default is 3
  parameters.numberParticles = 2000;
  parameters.tolerance = 1.0; // "Smoothing Coefficient"
  parameters.resamplingThreshold = 500;
  parameters.distribution = 0.1f;
  parameters.translate = true;
  parameters.allowSegmentation = false;
  
  // COEFFICIENTS
  
  ofxGVF::ofxGVFVarianceCoefficents coefficients;
  
  coefficients.phaseVariance      = 0.00005;
  coefficients.speedVariance      = 0.001;
  coefficients.scaleVariance      = 0.00001;
  coefficients.rotationVariance   = 0.000001;


  // BUILDING OBJECT
  
  mygvf = new ofxGVF();                       // could use mygvf = new ofxGVF(parameters, coefficients);
  mygvf->setup(parameters, coefficients);     // but 'setup' is the 'OF' way ;)

}

gvfKinectHandler::~gvfKinectHandler()
{
  if(mygvf != NULL)
    delete mygvf;
}

void gvfKinectHandler::addTemplate() {
  mygvf->addTemplate();
}


void gvfKinectHandler::gvf_data(ofPoint p)
{
  int state = mygvf->getState();
  
  if(state == ofxGVF::STATE_CLEAR)
  {
    return;
  }
  if(state == ofxGVF::STATE_LEARNING)
  {
    // Create vector from input point.
    vector<float> vect(3);
    for (int k = 0; k < 3; k++)
      vect[k] = p[k];
    
    // Fill template (The last template)
    mygvf->fillTemplate(mygvf->getNumberOfTemplates() - 1, vect);
    
  }
  else if(state == ofxGVF::STATE_FOLLOWING) // If the state if FOLLOWING
  {
    // create a vector with input dimension arguments
    vector<float> vect(3);
    
    // fill the vector with the incoming data
    for (int k = 0; k < 3; k++)
      vect[k] = p[k];
    
    // make inference
    mygvf->infer(vect);
  }
}

//-----------------------------------------------------------------------
// MARK: Sets
//-----------------------------------------------------------------------
void gvfKinectHandler::setState(ofxGVF::ofxGVFState state) {
  mygvf->setState(state);
}

void gvfKinectHandler::setSmoothingCoefficient(float smoothingCoeficient)
{
  mygvf->setTolerance(smoothingCoeficient);
}

void gvfKinectHandler::setResamplingThreshold(int resamplingThreshold)
{
  mygvf->setResamplingThreshold(resamplingThreshold);
}

void gvfKinectHandler::setAdaptSpeed(ofxGVF::ofxGVFVarianceCoefficents varianceCoefficients)
{
  mygvf->setVarianceCoefficents(varianceCoefficients);
}

void gvfKinectHandler::setNumberOfParticles(int numParticles)
{
  mygvf->setNumberOfParticles(numParticles);
}

//-----------------------------------------------------------------------
// MARK: Gets
//-----------------------------------------------------------------------
ofxGVF::ofxGVFState gvfKinectHandler::getState() {
  return mygvf->getState();
}

int gvfKinectHandler::getTemplateCount()
{
  return mygvf->getNumberOfTemplates();
}

int gvfKinectHandler::getIndexMostProbable()
{
  return mygvf->getMostProbableGestureIndex();
}

