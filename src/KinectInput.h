//
//  KinectInput.h
//  mvf
//
//  Created by Alejandro Van Zandt-Escobar on 03/04/2014.
//
//

#ifndef __mvf__KinectInput__
#define __mvf__KinectInput__

#include <iostream>

#include "NiTE.h"


#define OUTPUT_BUFFER_SIZE 1024 //????
//----------

#define MAX_DEPTH 10000

class KinectInput
{
public:
	KinectInput(const char* strSampleName);
	virtual ~KinectInput();
  
	virtual openni::Status Init(int argc, char **argv);
	virtual openni::Status Run();	//Does not return
  
protected:
	virtual void Display();
	virtual void DisplayPostDraw(){};	// Overload to draw over the screen image
  
	virtual void OnKey(unsigned char key, int x, int y);
  
	virtual openni::Status InitOpenGL(int argc, char **argv);
	void InitOpenGLHooks();
  
	void Finalize();
  
private:
	KinectInput(const KinectInput&);
	KinectInput& operator=(KinectInput&);
  
	static KinectInput* ms_self;
	static void glutIdle();
	static void glutDisplay();
	static void glutKeyboard(unsigned char key, int x, int y);
  
	float				m_pDepthHist[MAX_DEPTH];
	char			m_strSampleName[ONI_MAX_STR];
	openni::RGB888Pixel*		m_pTexMap;
	unsigned int		m_nTexMapX;
	unsigned int		m_nTexMapY;
  
	openni::Device		m_device;
	nite::UserTracker* m_pUserTracker;
  
	nite::UserId m_poseUser;
	uint64_t m_poseTime;
  
  void SendSkeleton(nite::UserTracker* pUserTracker, const nite::UserData& userData);
  void SendLimb(const nite::SkeletonJoint& joint, int user_id, int joint_id);
  void SendCenterOfMass(nite::UserTracker* pUserTracker, const nite::UserData& userData);
  void SendBoundingBox(nite::UserTracker* pUserTracker, const nite::UserData& userData);
  
  
};

#endif /* defined(__mvf__KinectInput__) */
