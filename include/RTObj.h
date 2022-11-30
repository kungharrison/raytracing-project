#include "RTGeometry.h"
#include "Triangle.h"
#ifndef __RTOBJ_H__
#define __RTOBJ_H__

class RTObj : public RTGeometry {
public:
	void init(const char* filename);
};
#endif