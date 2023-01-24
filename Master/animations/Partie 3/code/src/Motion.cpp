#include "Motion.h"
#include <fstream>
#include <iostream>
#include <cstdlib>
using namespace std;

Motion::Motion(const std::string & filename) : _nbFrames(0), _frequency(0), _nbJoints(0) {
    ifstream datafile (filename.c_str());
    if (!datafile.is_open()) {
            cout << "ERROR: unable to open file: " << filename << endl;
            system("pause");
            exit(1);
    }
    else {
        datafile >> _nbFrames >> _frequency >> _nbJoints;
        if (_nbFrames!=0 && _frequency!=0 && _nbJoints!=0) {
            float jointAngle;
            for (unsigned int f=0; f<_nbFrames; f++) {
                std::vector<float> frameData;
                for (unsigned int j=0; j<_nbJoints; j++) {
                    datafile >> jointAngle;
                    frameData.push_back(jointAngle);
                }
                _motionData.push_back(frameData);
            }
        }
    }
}

std::vector<float> Motion::getMotionDataAtFrame(unsigned int frameIndex) const {
    std::vector<float> res;
    if (frameIndex < _nbFrames)
        res = _motionData[frameIndex];
    return res;
}

float Motion::getMotionDataAtFrameJoint(unsigned int frameIndex, unsigned int joint) const {
    float res = 0.0;
    if (frameIndex < _nbFrames && joint < _nbJoints)
        res = (_motionData[frameIndex])[joint];
    return res;
}
