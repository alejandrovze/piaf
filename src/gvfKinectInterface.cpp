//
//  gvfKinectInterface.cpp
//  mvf
//
//  Created by Alejandro Van Zandt-Escobar on 02/04/2014.
//
//

#include "gvfKinectInterface.h"


// For kinect depth image
#define MAX_DEPTH 10000


//--------------------------------------------------------------
void gvfKinectInterface::setup(gvfKinect* _kinect_app) {
  
  status_gui = new ofxUICanvas();
  
  kinect_app = _kinect_app;
  
  InitialiseKinectGui();
  
}

//--------------------------------------------------------------
void gvfKinectInterface::exit() {
  
  delete status_gui;
}

//--------------------------------------------------------------

void gvfKinectInterface::update() {
  
//  UpdateKinectGui();
  
}

//--------------------------------------------------------------
void gvfKinectInterface::draw() {
  
  UpdateKinectGui();
  
}

//--------------------------------------------------------------
//MARK: Status Gui
//--------------------------------------------------------------

void gvfKinectInterface::initialiseStatusGui() {

//  statusGui.setup("ofxGVF Status");
//
//  statusGui.add(inputOnOff.set("Recording",false));
//  statusGui.add(currentState.set("Current State", "XXX"));
//
//  mostProbableStatus.setName("Most Probable Template");
//  mostProbableStatus.add(mostProbableIndex.set("ID", -1, -1, 10));
//  mostProbableStatus.add(mostProbablePhase.set("Phase", 0., 0., 1.));
//  mostProbableStatus.add(mostProbableSpeed.set("Speed", 0., -1., 1.));
//  mostProbableStatus.add(mostProbableScale.set("Scale", 0., 0., 1.));
//
//  statusGui.add(mostProbableStatus);
//
//  statusGui.setShape(ofRectangle(0, 220, 250, 100));
//  statusGui.setPosition(0,0);
//  statusGui.setWidthElements(250);
}

//--------------------------------------------------------------
void gvfKinectInterface::updateStatusGui(){

//  inputOnOff.set(isFollowing);
//
//  string state_string = "XXX";
//
//  switch(state) {
//    case ofxGVF::STATE_LEARNING:
//      state_string = "Learning";
//      break;
//    case ofxGVF::STATE_FOLLOWING:
//      state_string = "Following";
//      break;
//    case ofxGVF::STATE_CLEAR:
//      state_string = "Clear";
//      break;
//  }
//
//  currentState.set(state_string);
//
//  mostProbableIndex.set(mostProbable);
//  mostProbablePhase.set(infoMostProbable.phase);
//  mostProbableSpeed.set(infoMostProbable.speed);
//  //    mostProbableScale.set(infoMostProbable.scale); // ???: position not updated
  
}


//
////--------------------------------------------------------------
//void gvfPianoInterface::setup(gvfPianoApp* _piano_follower_p, int _nAccelerometers){
//  
//  piano_follower_p = _piano_follower_p;
//  
//  nAccelerometers = _nAccelerometers; // !!!: Shouldn't be using this.
//  
//  initialiseSettingsGui();
//  initialiseStatusGui();
//  initialiseInputsGui();
//  
//  initialiseGestureGui(piano_follower_p->getGestureData());
//  
//  initialiseTemplatesGui();
//  
//}
//
////--------------------------------------------------------------
//void gvfPianoInterface::draw(){
//  
//  settingsGui.draw();
//  inputsGui.draw();
//  statusGui.draw();
//  
//}
//
////--------------------------------------------------------------
//void gvfPianoInterface::update(){
//  
//  updateInputsGui(piano_follower_p->getInputData());
//  
//  updateStatusGui(piano_follower_p->getState(),
//                  piano_follower_p->getFollowing(),
//                  piano_follower_p->getIndexMostProbable(),
//                  piano_follower_p->getInfoOfMostProbable());
//  
//  updateGestureGui();
//  
//  if (templates.size() != piano_follower_p->getTemplatesLearned())
//    updateTemplatesGui(piano_follower_p->getTemplatesLearned());
//  
//  // Update templates after gesture has been learned only.
//  if (piano_follower_p->getState() == ofxGVF::STATE_FOLLOWING) {
//    updateTemplates(piano_follower_p->getTemplatesLearned());
//  }
//  
//  templates_gui->draw();
//}
//
//void gvfPianoInterface::exit() {
//  inputSliders.clear();
//}
//
//
////--------------------------------------------------------------
////MARK: Settings GUI
////--------------------------------------------------------------
//
////--------------------------------------------------------------
//void gvfPianoInterface::initialiseSettingsGui(){
//  
//  // the initial parameters values set here are not connected to the initial parameters values on the gvfhandler constructor
//  // those are the real initial values. The values here will only take effect once the user changes them
//  // !!!: (this is inconsistent and should be fixed)
//  
//  settingsGui.setup("ofxGVF Settings");
//  
//  settings.setName("Settings");
//  settings.add(numParticles.set("Number of particles", 2000, 10, 10000));
//  settings.add(resampleThreshold.set("Resampling threshold", 1000, 100, 10000));
//  settings.add(smoothingCoefficient.set("Smoothing coefficient", 0.2, 0.01, 2.0));
//  
//  varianceCoefficients.setName("Variance Coefficients");
//  
//  varianceCoefficients.add(sigPosition.set("Position", 0.0001, 0.000001, 0.1));
//  varianceCoefficients.add(sigSpeed.set("Speed", 0.01, 0.000001, 0.1));
//  varianceCoefficients.add(sigScale.set("Scale", 0.0001, 0.000001, 0.1));
//  varianceCoefficients.add(sigRotation.set("Rotation", 0.0001, 0.000001, 0.1));
//  
//  settings.add(varianceCoefficients);
//  
//  settingsGui.add(settings);
//  
//  settingsGui.add(save.setup("save gestures"));
//  settingsGui.add(load.setup("load gestures"));
//  
//  settingsGui.setShape(ofRectangle(30, 110, 250, 100));
//  settingsGui.setPosition(0, 110);
//  settingsGui.setWidthElements(250);
//  
//  numParticles.addListener(this, &gvfPianoInterface::numParticlesChanged);
//  resampleThreshold.addListener(this, &gvfPianoInterface::resampleThresholdChanged);
//  smoothingCoefficient.addListener(this, &gvfPianoInterface::smoothingCoefficientChanged);
//  
//  sigPosition.addListener(this, &gvfPianoInterface::varianceCoefficentsChanged);
//  sigSpeed.addListener(this, &gvfPianoInterface::varianceCoefficentsChanged);
//  sigScale.addListener(this, &gvfPianoInterface::varianceCoefficentsChanged);
//  sigRotation.addListener(this, &gvfPianoInterface::varianceCoefficentsChanged);
//  
//  save.addListener(this, &gvfPianoInterface::saveGestures);
//  load.addListener(this, &gvfPianoInterface::loadGestures);
//  
//}
//
////--------------------------------------------------------------
//void gvfPianoInterface::numParticlesChanged(int & numParticles) {
//  piano_follower_p->setNumberOfParticles(numParticles);
//}
//
////--------------------------------------------------------------
//void gvfPianoInterface::resampleThresholdChanged(int & resampleThreshold) {
//  
//  piano_follower_p->setResamplingThreshold(resampleThreshold);
//}
//
////--------------------------------------------------------------
//void gvfPianoInterface::smoothingCoefficientChanged(float & smoothingCoefficient) {
//  
//  piano_follower_p->setSmoothingCoefficient(smoothingCoefficient);
//}
//
////--------------------------------------------------------------
//void gvfPianoInterface::varianceCoefficentsChanged(float & coefficent) {
//  
//  ofxGVF::ofxGVFVarianceCoefficents sigs;
//  sigs.phaseVariance = sigPosition.get();
//  sigs.speedVariance = sigSpeed.get();
//  sigs.scaleVariance = sigScale.get();
//  sigs.rotationVariance = sigRotation.get();
//  
//  piano_follower_p->setAdaptSpeed(sigs);
//}
//

//
////--------------------------------------------------------------
////MARK: Inputs GUI
////--------------------------------------------------------------
//// TODO: Generalize
//
////--------------------------------------------------------------
//void gvfPianoInterface::initialiseInputsGui(){
//  
//  inputsGui.setup("Data Inputs");
//  
//  inputsInfo = piano_follower_p->getInputsInfo();
//  
//  inputSliders.reserve(inputsInfo.size());
//  
//  // !!!: Potentially dangerous memory management.
//  for (std::vector<gvfInputInfo>::iterator it = inputsInfo.begin();
//       it != inputsInfo.end(); ++it) {
//    
//    inputSliders.push_back(ofxVec3SliderDisplay());
//    inputsGui.add(inputSliders.back().setup(it->inputName, ofVec3f(0.3,0.1, -0.1),
//                                            ofVec3f(-1.,-1.,-1.), ofVec3f(1.,1.,1.))); //TODO: complete.
//    
//    cout << it->inputName << "\n";
//  }
//  
//  inputsGui.setShape(ofRectangle(30, 110, 250, 100));
//  inputsGui.setPosition(0, 330);
//  inputsGui.setWidthElements(250);
//  
//}
//
//// FIXME: Using ofVec3f, not the most generalizable, should be normal vectors really.
////--------------------------------------------------------------
//void gvfPianoInterface::updateInputsGui(vector<float> inputData){
//  
//  // We are assuming inputs are all 3-dimensional (for now...)
//  for (int i = 0; i < inputsInfo.size(); i++) {
//    ofVec3f updated_input(inputData.at(i*3+0), inputData.at(i*3+1), inputData.at(i*3+2));
//    inputSliders.at(i).displayValue(updated_input);
//  }
//  
//}
//
////--------------------------------------------------------------
////MARK: Gestures GUI
////--------------------------------------------------------------
//void gvfPianoInterface::initialiseGestureGui(vector<vector<float> >* gesture_buffer) {
//  
//  int graphWidth = 800;
//  int graphHeight = 120;
//  int maxPhraseLength = 410000;
//  
//  string graph_name = "Current Gesture Graph";
//  
//  gesture_gui = new ofxUICanvas(250, 0, graphWidth, graphHeight);
//  
//  gesture_gui->setColorBack(ofColor::grey);
//  gesture_gui->setDrawBack(true);
//  gesture_gui->setPadding(0);
//  
//  //FIXME: Sizing of the graph?
//  current_gesture_graph = new ofxUIMovingGraphX(graphWidth - 50, graphHeight - 50, gesture_buffer,
//                                                maxPhraseLength, -1., 1., graph_name);
//  
//  
//  current_gesture_graph->setDrawBack(true);
//  current_gesture_graph->setColorBack(ofColor::green);
//  
//  gesture_gui->addWidgetDown(current_gesture_graph);
//  
//}
//
//void gvfPianoInterface::updateGestureGui() {
//  current_gesture_graph->draw();
//}
//
//
////--------------------------------------------------------------
////MARK: Templates GUI
////--------------------------------------------------------------
//void gvfPianoInterface::initialiseTemplatesGui() {
//  
//  int graphWidth = 800;
//  int graphHeight = 800;
//  
//  templates_gui = new ofxUICanvas(250, 120, graphWidth, graphHeight);
//  
//  templates_gui->setColorBack(ofColor::paleTurquoise);
//  templates_gui->setDrawBack(true);
//  
//  // add a listener (for audio mostly)
//  // ofAddListener(templatesCanvas->newGUIEvent,this,&ofApp::templatesGuiEvent);
//  
//  
//}
//
////--------------------------------------------------------------
//void gvfPianoInterface::updateTemplatesGui(int nTemplates) {
//  
//  
//  // ???: Dyamic sizing or scrolling?
//  
//  int graphWidth = 800;
//  int graphHeight = 100;
//  int maxPhraseLength = 41000;
//  
//  templates.resize(nTemplates);
//  templates_gui->removeWidgets();
//  
//  for (int i = 0; i < nTemplates; ++i) {
//    
//    // ???: We copy the vector, perhaps can do without copying.
//    templates.at(i).data.resize(piano_follower_p->getTemplateData(i).size(),
//                                vector<float>(piano_follower_p->getTemplateData(i).at(0).size()));
//    templates.at(i).data = piano_follower_p->getTemplateData(i);
//    // Add Graph of data (static data)
//    templates.at(i).graph = new ofxUIMovingGraphX(graphWidth - 10, graphHeight,
//                                                  &templates.at(i).data, maxPhraseLength, -1., 1., "graph");
//    templates.at(i).graph->setDrawBack(true);
//    templates.at(i).graph->setColorBack(ofColor::green);
//    // Add Phase Slider (dynamic)
//    templates_gui->addWidgetDown(templates.at(i).graph);
//    
//    templates.at(i).phase_slider = new ofxUISlider("slider", 0., 1., &templates.at(i).phase, graphWidth - 10, 20);
//    templates.at(i).phase_slider->setLabelVisible(false);
//    templates.at(i).phase_slider->setColorFill(ofColor::grey);
//    templates_gui->addWidgetDown(templates.at(i).phase_slider);
//  }
//}
//
////--------------------------------------------------------------
//void gvfPianoInterface::updateTemplates(int nTemplates) {
//  for (int i = 0; i < nTemplates; ++i) {
//    templates.at(i).phase = piano_follower_p->getTemplateInfo(i).phase;
//    templates.at(i).is_most_probable = (i == piano_follower_p->getIndexMostProbable());
//    if (templates.at(i).is_most_probable)
//      templates.at(i).phase_slider->setColorFill(ofColor::white);
//    else
//      templates.at(i).phase_slider->setColorFill(ofColor::grey);
//  }
//}
//
////
////// TODO: After writing proper draw function for gesture, call here.
////void gvfhandler::drawTemplates(float scale, int indexMostProbable, recognitionInfo infoMostProbable) {
////    int nTemplates = templates.size();
////    int px, py, w, h;
////
////    for(int i = 0; i < nTemplates; i++)
////    {
////        px = 300;
////        py = i * (h + 20);
////
////        ofRectangle templateRect = ofRectangle(px, py, w, h);
////        gvfGesture g = getTemplateGesture(i);
////        g.setAppearance(g.getColor(), 1.5, 255, 50, 1);
////        g.drawBoundaries = false;
////        if (i == indexMostProbable)
////            g.draw(templateRect, scale, infoMostProbable.probability);
////        else
////            g.draw(templateRect, scale);
////    }
////}
//
//
//// --------------------------------
//// MARK: Load/Save
//// --------------------------------
////--------------------------------------------------------------
//void gvfPianoInterface::saveGestures() {
//  
//  string filename;
//  
//  ofFileDialogResult dialogResult = ofSystemSaveDialog("my_gestures", "Save gestures");
//  if(!dialogResult.bSuccess)
//    return;
//  
//  stringstream ss;
//  ss << dialogResult.filePath;
//  filename = ss.str();
//  cout << filename;
//  
//  piano_follower_p->saveGestures(filename);
//  cout << "Gestures saved.\n";
//}
//
////--------------------------------------------------------------
//void gvfPianoInterface::loadGestures() {
//  string filename;
//  
//  ofFileDialogResult dialogResult = ofSystemLoadDialog("Select the file containing gesture data");
//  if(!dialogResult.bSuccess)
//    return;
//  
//  stringstream ss;
//  ss << dialogResult.filePath;
//  filename = ss.str();
//  cout << filename;
//  
//  piano_follower_p->loadGestures(filename);
//  cout << "Gestures loaded.\n";
//}
//
//
//
//// !!!: Maybe just get rid of this
//void gvfPianoInterface::drawString() {
//  //
//  //    if(state == STATE_FOLLOWING){
//  //
//  //        //        state_string.append(" ]\nSTATE_FOLLOWING [X]\nSTATE_CLEAR     [ ]");
//  //
//  //        //        if(displayParticles)
//  //        //            gvfh.printParticleInfo(currentGesture);
//  //
//  //        // temp will have the partial representation of how gvf is recognising the gesture being performed
//  //        //        gvfGesture temp = gvfh.getRecognisedGestureRepresentation();
//  //        //        if(temp.isValid)
//  //        //        {
//  //        //            // the estimated gesture will be drawn on the same area
//  //        //            // as the gesture being performed and starting on the same point
//  //        ////            ofRectangle da = currentGesture.getDrawArea();
//  //        //            ofPoint p = currentGesture->getInitialOfPoint();
//  //        //
//  //        //            /*
//  //        //            if(displayEstimatedGesture)
//  //        //            {
//  //        //                temp.setAppearance(ofColor(0,255,0), 5, 255, 180, 1);
//  //        //                temp.draw(templatesScale);
//  //        //
//  //        //                temp.setDrawArea(da);
//  //        //                temp.setInitialPoint(p + 1);
//  //        //                temp.centraliseDrawing = false;
//  //        //                temp.draw();
//  //        //            }
//  //        //            */
//  //        //
//  //        //            /*
//  //        //            if(displayTemplate)
//  //        //            {
//  //        //                // draw the original template for the gesture being recognised
//  //        //                gvfGesture g = gvfh.getTemplateGesture(gvfh.getIndexMostProbable());
//  //        //                g.setDrawArea(da);
//  //        //                g.setInitialPoint(p + 1);
//  //        //                g.centraliseDrawing = false;
//  //        //
//  //        //                // the template's opacity is determined by how probable the recognition is
//  //        //                g.setAppearance(g.getColor(),
//  //        //                                1.5, 255, 50,
//  //        //                                ofLerp(1/gvfh.getTemplateCount(),
//  //        //                                1,
//  //        //                                gvfh.getRecogInfoOfMostProbable().probability));
//  //        //               g.draw();
//  //        //            }
//  //        //             */
//  //        //        }
//  //
//  //    }
//}





//--------------------------------------------------------------
// MARK: Kinect Re-Working
//--------------------------------------------------------------


void gvfKinectInterface::InitialiseKinectGui() {
  
  kinect_width = 600;
  kinect_height = 400;
  
  depth_width = 640;
  depth_height = 480;

}

void gvfKinectInterface::UpdateKinectGui() {
  
  if (kinect_app->get_is_live()) {
    
    UpdateDepth(kinect_app->get_depth_frame());
    
    depth_image.draw(0, 0, kinect_width, kinect_height);
    
    UpdateSkeleton(kinect_app->get_depth_data_point());
    
  }
  
  DisplaySkeleton(kinect_app->get_current_point());
  
}

// TODO: Displaying Depth Frame
//--------------------------------------------------------------
void gvfKinectInterface::UpdateDepth(openni::VideoFrameRef depth_frame) {
  
  if (depth_frame.isValid()) {
    
    int res_x = depth_frame.getVideoMode().getResolutionX();
    int res_y = depth_frame.getVideoMode().getResolutionY();
    int numPixels = res_x * res_y;
    
    float* depth_histogram = CalculateHistogram(MAX_DEPTH, depth_frame);
    
    // Allocate image
    if (!depth_image.isAllocated()) {
      depth_image.allocate(res_x, res_y, OF_IMAGE_GRAYSCALE);
      
      grayPixels = new unsigned char[numPixels];
      memset(grayPixels, 0, numPixels * sizeof(unsigned char));
    }
    
    
    const openni::DepthPixel* depthPixels = (const openni::DepthPixel*) depth_frame.getData();
    
    for (int i = 0; i < numPixels; i++, depthPixels++) {
      grayPixels[i] = depth_histogram[*depthPixels];
    }
    
    depth_image.setFromPixels(grayPixels, res_x, res_y, OF_IMAGE_GRAYSCALE);
    
    int rowSize = depth_frame.getStrideInBytes() / sizeof(openni::DepthPixel);
    
  }
  
}

//--------------------------------------------------------------
float* gvfKinectInterface::CalculateHistogram(int histogramSize, const openni::VideoFrameRef& depthFrame)
{
	const openni::DepthPixel* pDepth = (const openni::DepthPixel*)depthFrame.getData();
	int width = depthFrame.getWidth();
	int height = depthFrame.getHeight();
  
  static float pHistogram[MAX_DEPTH];
  
	// Calculate the accumulative histogram (the yellow display...)
	memset(pHistogram, 0, histogramSize*sizeof(float));
	int restOfRow = depthFrame.getStrideInBytes() / sizeof(openni::DepthPixel) - width;
  
	unsigned int nNumberOfPoints = 0;
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x, ++pDepth)
		{
			if (*pDepth != 0)
			{
				pHistogram[*pDepth]++;
				nNumberOfPoints++;
			}
		}
		pDepth += restOfRow;
	}
	for (int nIndex = 1; nIndex < histogramSize; nIndex++)
	{
		pHistogram[nIndex] += pHistogram[nIndex-1];
	}
	if (nNumberOfPoints)
	{
		for (int nIndex = 1; nIndex < histogramSize; nIndex++)
		{
			pHistogram[nIndex] = (256 * (1.0f - (pHistogram[nIndex] / nNumberOfPoints)));
		}
	}
  
  return pHistogram;
  
}


//--------------------------------------------------------------
void gvfKinectInterface::UpdateSkeleton(SkeletonDataPoint new_point) {
  
  
  
  // Draw CENTER OF MASS
  int x_pos = (int) ((float) (new_point.center_of_mass.x) * (float) kinect_width / (float) depth_width);
  int y_pos = (int) ((float) (new_point.center_of_mass.y)* (float) kinect_height / (float) depth_height);
  
  ofNoFill();
  ofSetColor(0, 255, 255);
  ofCircle(x_pos, y_pos, 5);
  ofFill();
  
  
  // Draw SKELETON
  ofSetColor(255, 0, 255);
  
  for (int i = 0; i < N_JOINTS; ++i) {
    
    x_pos = (int) ((float) (new_point.joints[i].x) * (float) kinect_width / (float) depth_width);
    y_pos = (int) ((float) (new_point.joints[i].y) * (float) kinect_height / (float) depth_height);
    
    ofCircle(x_pos, y_pos, 5);
  }
  
  // TODO: Draw bounding box
  
}

// Diplaying a saved point
//--------------------------------------------------------------
void gvfKinectInterface::DisplaySkeleton(SkeletonDataPoint new_point) {
  
  ofSetColor(255, 255, 0);
  
  int x_center = 500;
  int y_center = 600;
  
  ofNoFill();
  ofSetColor(0, 255, 255);
  ofCircle(x_center, y_center, 100);
  ofFill();
  
  // Draw SKELETON
  ofSetColor(255, 0, 255);
  int x_pos;
  int y_pos;
  
  for (int i = 0; i < N_JOINTS; ++i) {
    
    x_pos = x_center + new_point.joints[i].x / 10.0;
    y_pos = y_center + new_point.joints[i].y / 10.0;
    
    ofCircle(x_pos, y_pos, 5);
  }
  
}

