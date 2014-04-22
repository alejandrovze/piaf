//
//  OSCSender.cpp
//  mvf
//
//  Created by Alejandro Van Zandt-Escobar on 11/04/2014.
//
//

#include "OSCInterface.h"


//--------------------------------------------------------------
void OSCInterface::setup(){
  
	// open an outgoing connection to HOST:PORT
	sender.setup(HOST, OUT_PORT);
  
  // listen on the given port
	cout << "listening for osc messages on port " << IN_PORT << "\n";
	receiver.setup(IN_PORT);
  
	current_msg_string = 0;
  
}

//--------------------------------------------------------------
void OSCInterface::update(){
  
  
  // SENDER
  
  ofxOscMessage m;
  m.setAddress("/test");
  m.addIntArg(1);
  m.addFloatArg(3.5f);
  m.addStringArg("hello");
  m.addFloatArg(ofGetElapsedTimef());
  sender.sendMessage(m);
  
  
  // RECEIVER
  
  
  // hide old messages
	for(int i = 0; i < NUM_MSG_STRINGS; i++){
		if(timers[i] < ofGetElapsedTimef()){
			msg_strings[i] = "";
		}
	}
  
	// check for waiting messages
	while(receiver.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(&m);
    
		// check for mouse moved message
		if(m.getAddress() == "/time"){
      clock_time = m.getArgAsFloat(0);
      cout << "TIME IS THE ENEMY " << clock_time << endl;
		}
		else{
			// unrecognized message: display on the bottom of the screen
			string msg_string;
			msg_string = m.getAddress();
			msg_string += ": ";
			for(int i = 0; i < m.getNumArgs(); i++){
				// get the argument type
				msg_string += m.getArgTypeName(i);
				msg_string += ":";
				// display the argument - make sure we get the right type
				if(m.getArgType(i) == OFXOSC_TYPE_INT32){
					msg_string += ofToString(m.getArgAsInt32(i));
				}
				else if(m.getArgType(i) == OFXOSC_TYPE_FLOAT){
					msg_string += ofToString(m.getArgAsFloat(i));
				}
				else if(m.getArgType(i) == OFXOSC_TYPE_STRING){
					msg_string += m.getArgAsString(i);
				}
				else{
					msg_string += "unknown";
				}
			}
			// add to the list of strings to display
			msg_strings[current_msg_string] = msg_string;
			timers[current_msg_string] = ofGetElapsedTimef() + 5.0f;
			current_msg_string = (current_msg_string + 1) % NUM_MSG_STRINGS;
			// clear the next line
			msg_strings[current_msg_string] = "";
		}
    
	}

}

// returns position in phrase
int OSCInterface::get_time(int phrase_length) {
  
  int time = ((int) clock_time) % phrase_length;
  
  return time;
}

int OSCInterface::get_clock() {
  return clock_time;
}

