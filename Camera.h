#ifndef CAMERA_H
#define CAMERA_H
#include "ImagePlane.h"
#include <string.h>
class Camera
{ 
public:
	int camid;
	float position[3];
	float gaze[3];
	float up[3];
	ImagePlane plane;
	std::string filename;
	
};
 
#endif
