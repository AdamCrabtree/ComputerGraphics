
#include "vmath.h"

/*
* routine to generate an orthographic projection matrix.
* author:  Jerry Heuring
* Date:  September 28, 2018
*
* The routine sets up an orthonormal projection.
*
* Bugs:
*    None Known.
*
* Revisions:
*
*/
vmath::mat4 ortho(float znear = -100.0f, float zfar = 100.0,
	float left = -1.0, float right = 1.0,
	float bottom = -1.0, float top = 1.0) {
	vmath::mat4 orthoMatrix = vmath::scale(1.0f);
	orthoMatrix[0][0] = 2.0f / (right - left);
	orthoMatrix[1][1] = 2.0f / (top - bottom);
	orthoMatrix[2][2] = 2.0f / (zfar - znear);
	orthoMatrix[0][3] = -(right + left) / (right - left);
	orthoMatrix[1][3] = -(top + bottom) / (top - bottom);
	orthoMatrix[2][3] = -(zfar + znear) / (zfar - znear);
	return orthoMatrix;
}