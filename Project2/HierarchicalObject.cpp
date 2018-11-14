#include "HierarchicalObject.h"

/*
* The methods that go with the Hierarchical object.  Most are
* extremely simple at this point.  Need to test if I should be
* pre or post multiplying the transformation matrices.
*/


void HierarchicalObject::add(HierarchicalObject *obj) {
	subobjects.push_back(obj);
}

void HierarchicalObject::remove(HierarchicalObject *obj) {
	subobjects.remove(obj);
}

void HierarchicalObject::translate(float dx, float dy, float dz) {
	currentTransform = currentTransform * vmath::translate(vmath::vec3(dx, dy, dz));
}

void HierarchicalObject::scale(float sx, float sy, float sz) {
	currentTransform = currentTransform * vmath::scale(sx, sy, sz);
}

void HierarchicalObject::rotate(float angle, float vx, float vy, float vz) {
	currentTransform = currentTransform * vmath::rotate(angle, vmath::vec3(vx, vy, vz));
}

