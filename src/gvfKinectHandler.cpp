//
//  gvfKinectHandler.cpp
//  mvf
//
//  Created by Alejandro Van Zandt-Escobar on 28/03/2014.
//
//

#include "gvfKinectHandler.h"

gvfKinectHandler::gvfKinectHandler(string name, BodyPart feature, int n_dimensions):
gvf_name(name),
gvf_feature(feature)
{
  // initialisation of parameters. Functions are provided to change most of them at runtime
  
  // PARAMETERS
  ofxGVF::ofxGVFParameters parameters;
  
  parameters.inputDimensions = n_dimensions; // MARK: input dimensions default is 3
  parameters.numberParticles = 300;
  parameters.tolerance = 1.0; // "Smoothing Coefficient"
  parameters.resamplingThreshold = 500;
  parameters.distribution = 0.1f;
  parameters.translate = true;
  parameters.allowSegmentation = false; // Results seem to be better when true.
  
  
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
  
  gvf_data(get_body_part(data_point, gvf_feature));

}

//void gvfKinectHandler::gvf_data(ofPoint p) {
//  vector<float> vect(3);
//  for (int k = 0; k < 3; k++)
//    vect[k] = p[k];
//  gvf_data(vect);
//}
//
//void gvfKinectHandler::gvf_data(float f) {
//  vector<float> vect(1);
//  vect[0] = f;
//  gvf_data(vect);
//}

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
  
  // TODO
  switch (part) {
      
    case HEAD: {
      vector<float> head_point(3);
      for (int k = 0; k < 3; k++)
        head_point[k] =  data_point.joints[JOINT_HEAD][k];
      return head_point;
      break;
    }
      
    case RIGHT_SHOULDER: {
      ofVec3f right_torso = get_segment(data_point.joints[JOINT_RIGHT_SHOULDER], data_point.joints[JOINT_RIGHT_HIP]);
      ofVec3f right_arm = get_segment(data_point.joints[JOINT_RIGHT_ELBOW], data_point.joints[JOINT_RIGHT_SHOULDER]);
      vector<float> right_shoulder = vector<float>(1, get_angle(right_arm, right_torso) / 180.0);
      cout << "right shoulder " << right_shoulder[0] << endl;
      return right_shoulder;
      break;
    }
      
    case LEFT_SHOULDER: {
      ofVec3f left_torso = get_segment(data_point.joints[JOINT_LEFT_SHOULDER], data_point.joints[JOINT_LEFT_HIP]);
      ofVec3f left_arm = get_segment(data_point.joints[JOINT_LEFT_ELBOW], data_point.joints[JOINT_LEFT_SHOULDER]);
      vector<float> left_shoulder = vector<float>(1, get_angle(left_arm, left_torso) / 180.0);
      cout << "left shoulder " << left_shoulder[0] << endl;
      return left_shoulder;
      break;
    }
      
    case RIGHT_ELBOW: {
      ofVec3f right_forearm = get_segment(data_point.joints[JOINT_RIGHT_HAND], data_point.joints[JOINT_RIGHT_ELBOW]);
      ofVec3f right_arm = get_segment(data_point.joints[JOINT_RIGHT_ELBOW], data_point.joints[JOINT_RIGHT_SHOULDER]);
      vector<float> right_elbow = vector<float>(1, get_angle(right_forearm, right_arm) / 180.0);
      cout << "right elbow " << right_elbow[0] << endl;
      return right_elbow;
      break;
    }
      
    case LEFT_ELBOW: {
      ofVec3f left_forearm = get_segment(data_point.joints[JOINT_LEFT_HAND], data_point.joints[JOINT_LEFT_ELBOW]);
      ofVec3f left_arm = get_segment(data_point.joints[JOINT_LEFT_ELBOW], data_point.joints[JOINT_LEFT_SHOULDER]);
      vector<float> left_elbow = vector<float>(1, get_angle(left_forearm, left_arm) / 180.0);
      cout << "left elbow " << left_elbow[0] << endl;
      return left_elbow;
      break;
    }
      
    case RIGHT_KNEE: {
      ofVec3f right_calf = get_segment(data_point.joints[JOINT_RIGHT_FOOT], data_point.joints[JOINT_RIGHT_KNEE]);
      ofVec3f right_thigh = get_segment(data_point.joints[JOINT_RIGHT_KNEE], data_point.joints[JOINT_RIGHT_HIP]);
      vector<float> right_knee = vector<float>(1, get_angle(right_calf, right_thigh) / 180.0);
      cout << "right knee " << right_knee[0] << endl;
      return right_knee;
      break;
    }
      
    case LEFT_KNEE: {
      ofVec3f left_calf = get_segment(data_point.joints[JOINT_LEFT_FOOT], data_point.joints[JOINT_LEFT_KNEE]);
      ofVec3f left_thigh = get_segment(data_point.joints[JOINT_LEFT_KNEE], data_point.joints[JOINT_LEFT_HIP]);
      vector<float> left_knee = vector<float>(1, get_angle(left_calf, left_thigh) / 180.0);
      cout << "left knee " << left_knee[0] << endl;
      return left_knee;
      break;
    }
      
    case RIGHT_HAND: {
      vector<float> right_hand = vector<float>(3);
      
      right_hand[0] = data_point.joints[JOINT_RIGHT_HAND].x / 100.;
      right_hand[1] = data_point.joints[JOINT_RIGHT_HAND].y / 100.;
      right_hand[2] = data_point.joints[JOINT_RIGHT_HAND].z / 100.;
      
      cout << "right " << right_hand[0] << " " << right_hand[1] << " " << right_hand[2] << endl;
      
      return right_hand;
      break;
    }
      
    case LEFT_HAND: {
      vector<float> left_hand = vector<float>(3);
      
      left_hand[0] = data_point.joints[JOINT_LEFT_HAND].x / 100.;
      left_hand[1] = data_point.joints[JOINT_LEFT_HAND].y / 100.;
      left_hand[2] = data_point.joints[JOINT_LEFT_HAND].z / 100.;
      
      cout << "left " << left_hand[0] << " " << left_hand[1] << " " << left_hand[2] << endl;
      
      return left_hand;
      break;
    }
      
  }

}



//-----------------------------------------------------------------------
