#ifndef __SIMPLEOBJECT_H__
#define __SIMPLEOBJECT_H__
#include "HierarchicalObject.h"
#include <glew.h>
#include <glut.h>
/*
* Simple object -- used to draw a plane of triangles for demonstration purposes.
*
* The constructor requires a programID to create other pieces.
*
* Bugs:
*
* Revisions:
*     Checked on finding attributes and printed out lots of error messages if they weren't found. (annoying)
*     Fixed a bug with offset in addSubObject
*     Changed polymode to GL_LINE.
*     Fixed some problems with vertex coordinates and changed polymode back to GL_FILL
*
*/
class SimpleObject : public HierarchicalObject {
private:
	void addSubBuffer(GLfloat *data, int bufferSize, int attribLoc, int &offset, int count = 4);
	void buildObject(GLfloat *vertices, int nbrVertices, int vertexAttribLoc,
		GLfloat *normals, int nbrNormals, int normalAttribLoc,
		GLfloat *colors, int nbrColors, int colorsAttribLoc,
		GLuint & vao, GLuint &bao);
protected:
	GLuint programID, vaobjectID, baobjectID;
	int triangleCount;

public:
	SimpleObject(GLuint programID) {
		static GLfloat vertices[] = {
			-10.0f,   0.0f,  10.0f,   1.0f,   0.0f,   0.0f,  10.0f,   1.0f, -10.0f,   0.0f,   0.0f,   1.0f,
			0.0f,   0.0f,  10.0f,   1.0f,   0.0f,   0.0f,   0.0f,   1.0f, -10.0f,   0.0f,   0.0f,   1.0f,
			0.0f,   0.0f,  10.0f,   1.0f,  10.0f,   0.0f,   0.0f,   1.0f,   0.0f,   0.0f,   0.0f,   1.0f,
			0.0f,   0.0f,  10.0f,   1.0f,  10.0f,   0.0f,  10.0f,   1.0f,  10.0f,   0.0f,   0.0f,   1.0f,
			-10.0f,   0.0f, -10.0f,   1.0f, -10.0f,   0.0f,   0.0f,   1.0f,   0.0f,   0.0f, -10.0f,   1.0f,
			0.0f,   0.0f, -10.0f,   1.0f, -10.0f,   0.0f,   0.0f,   1.0f,   0.0f,   0.0f,   0.0f,   1.0f,
			0.0f,   0.0f,   0.0f,   1.0f,   0.0f,   0.0f, -10.0f,   1.0f,  10.0f,   0.0f,   0.0f,   1.0f,
			0.0f,   0.0f, -10.0f,   1.0f,  10.0f,   0.0f, -10.0f,   1.0f,  10.0f,   0.0f,   0.0f,   1.0f
		};
		static GLfloat colors[] = { 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
			0.3f, 0.3f, 0.3f, 1.0f, 0.3f, 0.3f, 0.3f, 1.0f, 0.3f, 0.3f, 0.3f, 1.0f
		};
		static GLfloat normals[] = { 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f
		};
		GLint positionAttribLoc, normalAttribLoc, colorAttribLoc;
		this->programID = programID;
		triangleCount = sizeof(vertices) / sizeof(GLfloat) / 4 / 3;
		positionAttribLoc = glGetAttribLocation(programID, "vPosition");
		if (positionAttribLoc == -1) {
			cerr << "Couldn't find vPosition attribute in shader\n";
		}
		normalAttribLoc = glGetAttribLocation(programID, "vNormal");
		if (normalAttribLoc == -1) {
			cerr << "Couldn't find vNormal attribute in shader\n";
		}
		colorAttribLoc = glGetAttribLocation(programID, "vColor");
		if (colorAttribLoc == -1) {
			cerr << "Couldn't find vColor attribute in shader\n";
		}
		vaobjectID = 0;
		baobjectID = 0;
		// modified to remove normals and colors for debugging purposes. 
		buildObject(vertices, sizeof(vertices), positionAttribLoc,
			NULL, sizeof(normals), normalAttribLoc,
			colors, sizeof(colors), colorAttribLoc,
			vaobjectID, baobjectID);
	}
	void draw();
	void draw(vmath::mat4 transform);

};

#endif