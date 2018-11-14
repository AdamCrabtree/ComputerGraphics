#ifndef __DRAWABLE_H__
#define __DRAWABLE_H__
#include "vmath.h"
#include <iostream>
#include <string>
using namespace std;
/*
* This is an "interface" that is built upon.  It defines a "Drawable"
* as an object that contains two draw methods -- one that takes a
* transformation matrix and one that does not.  It is intended that the
* one that takes the transformation matrix will apply the transformation
* to the object being drawn.
*
* Author:  Jerry Heuring
* Date:  November 2015
*
* Bugs:
*
* Revisions:
*/
class Drawable {
private:
protected:
public:
	virtual void draw() = 0;
	virtual void draw(vmath::mat4 transform) = 0;

};

#endif