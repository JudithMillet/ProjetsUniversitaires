#ifndef MOTION_H
#define MOTION_H

#include <string>
#include <vector>

typedef unsigned long DWORD;

class Motion {

public:

    // Constructor with the filename containing the motion data
	Motion(const std::string & filename);

	// get the number of frames in the motion
	unsigned int getNbFrames() const {return _nbFrames;}

	// get the frequency of the motion data (in seconds)
	float getFrequency() const {return _frequency;}

	// get the data at given frame
	std::vector<float> getMotionDataAtFrame(unsigned int frameIndex) const;

	// get the data at given frame and joint
	float getMotionDataAtFrameJoint(unsigned int index, unsigned int joint) const;

	// get/set startingTime
	DWORD getStartTime() const {return _startTime;}
	void setStartTime(DWORD stime) {_startTime=stime;}

protected:

    unsigned int _nbFrames;     // number of frames
    float _frequency;           // motion frequency
    unsigned int _nbJoints;     // number of joints

    std::vector<std::vector<float> > _motionData;    // the motion data

    DWORD _startTime;		    // timer

};

#endif
