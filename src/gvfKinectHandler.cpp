//
//  gvfKinectHandler.cpp
//  mvf
//
//  Created by Alejandro Van Zandt-Escobar on 28/03/2014.
//
//

#include "gvfKinectHandler.h"

gvfKinectHandler::gvfKinectHandler(int id, string name, BodyPart feature, int n_dimensions):
gvf_id(id),
gvf_name(name),
gvf_feature(feature)
{
  // initialisation of parameters. Functions are provided to change most of them at runtime
  
  // PARAMETERS
  ofxGVF::ofxGVFParameters parameters;
  
  parameters.inputDimensions = n_dimensions; // MARK: input dimensions default is 3
  parameters.numberParticles = 2000;
  parameters.tolerance = 2.0; // "Smoothing Coefficient"
  parameters.resamplingThreshold = 500;
  parameters.distribution = 0.1f;
  parameters.translate = true;
  parameters.allowSegmentation = true; // Results seem to be better when true. 
  
  
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

void gvfKinectHandler::gvf_data(SkeletonDataPoint data_point) {
  
  // TODO
  switch (gvf_feature) {
      
//    case CENTER_OF_MASS:
//      gvf_data(data_point.center_of_mass);
//      break;
      
    case HEAD:
      gvf_data(data_point.joints[JOINT_HEAD]);
      break;
      
    case RIGHT_ELBOW: {
      ofVec3f right_forearm = get_segment(data_point.joints[JOINT_RIGHT_HAND], data_point.joints[JOINT_RIGHT_ELBOW]);
      ofVec3f right_arm = get_segment(data_point.joints[JOINT_RIGHT_ELBOW], data_point.joints[JOINT_RIGHT_SHOULDER]);
      float right_elbow = get_angle(right_forearm, right_arm);
      gvf_data(right_elbow);
      break;
    }
      
    case LEFT_ELBOW: {
      ofVec3f left_forearm = get_segment(data_point.joints[JOINT_LEFT_HAND], data_point.joints[JOINT_LEFT_ELBOW]);
      ofVec3f left_arm = get_segment(data_point.joints[JOINT_LEFT_ELBOW], data_point.joints[JOINT_LEFT_SHOULDER]);
      float left_elbow = get_angle(left_forearm, left_arm);
      gvf_data(left_elbow);
      break;
    }
      
    case RIGHT_KNEE: {
      ofVec3f right_calf = get_segment(data_point.joints[JOINT_RIGHT_FOOT], data_point.joints[JOINT_RIGHT_KNEE]);
      ofVec3f right_thigh = get_segment(data_point.joints[JOINT_RIGHT_KNEE], data_point.joints[JOINT_RIGHT_HIP]);
      float right_knee = get_angle(right_calf, right_thigh);
      gvf_data(right_knee);
      break;
    }
      
    case LEFT_KNEE: {
      ofVec3f left_calf = get_segment(data_point.joints[JOINT_LEFT_FOOT], data_point.joints[JOINT_LEFT_KNEE]);
      ofVec3f left_thigh = get_segment(data_point.joints[JOINT_LEFT_KNEE], data_point.joints[JOINT_LEFT_HIP]);
      float left_knee = get_angle(left_calf, left_thigh);
      gvf_data(left_knee);
      break;
    }
      
//    case BREADTH: {
//      ofVec3f breadth = data_point.joints[JOINT_LEFT_HAND] - data_point.joints[JOINT_RIGHT_HAND];
//      gvf_data(breadth);
//      break;
//    }

  }
}

void gvfKinectHandler::gvf_data(ofPoint p) {
  vector<float> vect(3);
  for (int k = 0; k < 3; k++)
    vect[k] = p[k];
  gvf_data(vect);
}

void gvfKinectHandler::gvf_data(float f) {
  vector<float> vect(1);
  vect[0] = f;
  gvf_data(vect);
}

void gvfKinectHandler::gvf_data(vector<float> vect)
{
  int state = mygvf->getState();
  
  if(state == ofxGVF::STATE_CLEAR)
  {
    return;
  }
  if(state == ofxGVF::STATE_LEARNING)
  {
    // Fill template (The last template)
    mygvf->fillTemplate(mygvf->getNumberOfTemplates() - 1, vect);
    
  }
  else if(state == ofxGVF::STATE_FOLLOWING) // If the state if FOLLOWING
  {
    // make inference
    mygvf->infer(vect);
    
//    cout << "index checker " << mygvf->getMostProbableGestureIndex() << endl;
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

//-----------------------------------------------------------------------
int gvfKinectHandler::getTemplateCount()
{
  return mygvf->getNumberOfTemplates();
}

//-----------------------------------------------------------------------
int gvfKinectHandler::getIndexMostProbable()
{
  return mygvf->getMostProbableGestureIndex();
}

//-----------------------------------------------------------------------
RecognitionInfo gvfKinectHandler::getRecogInfoOfMostProbable() {
  
  return getRecogInfo(getIndexMostProbable());
  
}

//-----------------------------------------------------------------------
RecognitionInfo gvfKinectHandler::getRecogInfo(int template_id) {
  
  RecognitionInfo status_info;
  
  if (template_id >= 0 && template_id < getTemplateCount()) {
  
    vector<float> status_vector = mygvf->getEstimatedStatus().at(template_id);
    
    cout << "Status vector legnth(test): " << status_vector.size() << endl;
    
    // TODO Fill status_info
    
    //cached most probable status [phase, speed, scale[, rotation], probability
    
    int length = status_vector.size();
    
    status_info.phase = status_vector[0];
    status_info.speed = status_vector[1];
    status_info.scale;
    status_info.rotation;
    
    status_info.probability = status_vector[length - 1];
    
  }
  
  return status_info;
  
}


//-----------------------------------------------------------------------
// MARK: Feature Extraction
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------

ofVec3f gvfKinectHandler::get_segment(ofPoint joint1, ofPoint joint2) {
  return ofVec3f(joint2 - joint1);
}

float gvfKinectHandler::get_angle(ofVec3f segment1, ofVec3f segment2) {
  return segment1.angle(- segment2);
}

vector<float> gvfKinectHandler::get_body_part(SkeletonDataPoint data_point, BodyPart part) {
  
}



//-----------------------------------------------------------------------
