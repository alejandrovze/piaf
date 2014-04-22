//
//  OSCInterface.h
//  mvf
//
//  Created by Alejandro Van Zandt-Escobar on 11/04/2014.
//
//

#ifndef __mvf__OSCInterface__
#define __mvf__OSCInterface__

#include <iostream>
#include "ofxOsc.h"

#define HOST "localhost"
#define IN_PORT 7400
#define OUT_PORT 12345
#define NUM_MSG_STRINGS 20

//--------------------------------------------------------
class OSCInterface {
  
public:
  
  void setup();
  void update();
  
  ofTrueTypeFont font;
  ofxOscSender sender;
  
  ofxOscReceiver receiver;
  
  int current_msg_string;
  string msg_strings[NUM_MSG_STRINGS];
  float timers[NUM_MSG_STRINGS];
  
  
  int get_time(int phrase_length);
  int get_clock();
  
  float clock_time;

  
};




#endif /* defined(__mvf__OSCInterface__) */
