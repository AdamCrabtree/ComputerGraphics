#ifndef __HIERARCHICALOBJECT_H__
#define __HIERARCHICALOBJECT_H__
#include "Drawable.h"
#include <list>

/*
* This is a base class for all of the hierarchical objects.  It implements a simple list
* via the STL to store the subobjects and a mat4 to hold a current transformation.  The
* methods provide the ability to rotate, translate, and scale the object and the associated
* subobjects.   In addition, subobjects may be added (add) or removed (remove) from the
* subobject list.
*
* Author:  Jerry Heuring
* Date : November 2015
*
* Bugs:
*
* Revisions:
*
* Todo:
*
*/
class HierarchicalObject : public Drawable {
private:
protected:
	list <HierarchicalObject *> subobjects;
	vmath::mat4 currentTransform;
public:
	HierarchicalObject() {
		currentTransform = vmath::mat4(vmath::vec4(1.0f, 0.0f, 0.0f, 0.0f),
			vmath::vec4(0.0f, 1.0f, 0.0f, 0.0f),
			vmath::vec4(0.0f, 0.0f, 1.0f, 0.0f),
			vmath::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	}
	void translate(float dx, float dy, float dz);
	void scale(float sx, float sy, float sz);
	void rotate(float angle, float vx, float vy, float vz);
	void add(HierarchicalObject * obj);
	void remove(HierarchicalObject * obj);
	int nbrSubobjects() { return subobjects.size(); }
};

#endif