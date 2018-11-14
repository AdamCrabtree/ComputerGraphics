#include "CubeObject.h"
/*
*  There are problems with this cube.  I'm not sure but I think several of the triangles are
*  incorrect.
*/
CubeObject::CubeObject(GLuint programID) {


	GLfloat vertices[] = {
		-0.5, -0.5, -0.5, 1.0,  0.5, -0.5, -0.5, 1.0,  0.5,  0.5, -0.5, 1.0,  // front and back faces
		-0.5, -0.5, -0.5, 1.0,  0.5,  0.5, -0.5, 1.0, -0.5,  0.5, -0.5, 1.0,
		-0.5, -0.5,  0.5, 1.0,  0.5, -0.5,  0.5, 1.0,  0.5,  0.5,  0.5, 1.0,
		-0.5, -0.5,  0.5, 1.0,  0.5,  0.5,  0.5, 1.0, -0.5,  0.5,  0.5, 1.0,
		-0.5, -0.5, -0.5, 1.0, -0.5,  0.5,  0.5, 1.0, -0.5,  0.5, -0.5, 1.0,  // sides
		-0.5, -0.5, -0.5, 1.0, -0.5,  0.5,  0.5, 1.0, -0.5, -0.5,  0.5, 1.0,
		0.5, -0.5, -0.5, 1.0,  0.5,  0.5,  0.5, 1.0,  0.5,  0.5, -0.5, 1.0,  // sides
		0.5, -0.5, -0.5, 1.0,  0.5,  0.5,  0.5, 1.0,  0.5, -0.5,  0.5, 1.0,
		-0.5, -0.5, -0.5, 1.0,  0.5, -0.5, -0.5, 1.0,  0.5, -0.5,  0.5, 1.0,  // bottom
		-0.5, -0.5, -0.5, 1.0,  0.5, -0.5,  0.5, 1.0, -0.5, -0.5,  0.5, 1.0,
		-0.5,  0.5, -0.5, 1.0,  0.5,  0.5, -0.5, 1.0,  0.5,  0.5, -0.5, 1.0,  // top
		-0.5,  0.5, -0.5, 1.0,  0.5,  0.5,  0.5, 1.0, -0.5,  0.5,  0.5, 1.0
	};
	GLfloat colors[] = {
		1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, // white
		0.5, 0.5, 0.5, 1.0, 0.5, 0.5, 0.5, 1.0, 0.5, 0.5, 0.5, 1.0, // gray
		1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 1.0, // red
		1.0, 0.5, 0.5, 1.0, 1.0, 0.5, 0.5, 1.0, 1.0, 0.5, 0.5, 1.0, // pink?
		0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, // green
		0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0,
		0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, // blue
		0.0, 0.5, 1.0, 1.0, 0.0, 0.5, 1.0, 1.0, 0.0, 0.5, 1.0, 1.0,
		1.0, 0.0, 1.0, 1.0, 1.0, 0.0, 1.0, 1.0, 1.0, 0.0, 1.0, 1.0, // magenta
		1.0, 0.5, 1.0, 1.0, 1.0, 0.5, 1.0, 1.0, 1.0, 0.5, 1.0, 1.0,
		1.0, 1.0, 0.0, 1.0, 1.0, 1.0, 0.0, 1.0, 1.0, 1.0, 0.0, 1.0, // yellow
		1.0, 1.0, 0.5, 1.0, 1.0, 1.0, 0.5, 1.0, 1.0, 1.0, 0.5, 1.0
	};
	GLfloat normals[108];

	GLint positionAttribLoc, normalAttribLoc, colorAttribLoc;
	this->programID = programID;
	triangleCount = sizeof(vertices) / sizeof(GLfloat) / 4 / 3;
	positionAttribLoc = glGetAttribLocation(programID, "vPosition");
	normalAttribLoc = glGetAttribLocation(programID, "vNormal");
	colorAttribLoc = glGetAttribLocation(programID, "vColor");
	vaobjectID = 0;
	baobjectID = 0;
	// modified to remove normals and colors for debugging purposes. 
	buildObject(vertices, sizeof(vertices), positionAttribLoc,
		NULL, sizeof(normals), normalAttribLoc,
		colors, sizeof(colors), colorAttribLoc,
		vaobjectID, baobjectID);
}

void CubeObject::addSubBuffer(GLfloat *data, int bufferSize, int attribLoc, int &offset, int count) {
	if (data == NULL) {  // No data to add to the subbuffer
		return;
	}
	glBufferSubData(GL_ARRAY_BUFFER, offset, bufferSize, data);
	GLenum err_code = glGetError();
	if (err_code != GL_NO_ERROR) {
		cerr << err_code << " " << gluErrorString(err_code) << endl;
	}
	glVertexAttribPointer(attribLoc, count, GL_FLOAT, GL_FALSE, 0, (void *)offset);
	glEnableVertexAttribArray(attribLoc);
	offset = offset + bufferSize;
}

void CubeObject::buildObject(GLfloat *vertices, int nbrVertices, int vertexAttribLoc,
	GLfloat *normals, int nbrNormals, int normalAttribLoc,
	GLfloat *colors, int nbrColors, int colorsAttribLoc,
	GLuint & vao, GLuint & bao) {
	int bufferSize;
	int offset;
	if (vao == 0) {
		glGenVertexArrays(1, &vao);
	}
	glBindVertexArray(vao);
	vaobjectID = vao;
	if (bao == 0) {
		glGenBuffers(1, &bao);
	}
	bufferSize = nbrVertices + nbrNormals + nbrColors;
	glBindBuffer(GL_ARRAY_BUFFER, bao);
	glBufferData(GL_ARRAY_BUFFER, bufferSize, NULL, GL_STATIC_DRAW);
	offset = 0;
	addSubBuffer(vertices, nbrVertices, vertexAttribLoc, offset);
	addSubBuffer(normals, nbrNormals, normalAttribLoc, offset, 3);
	addSubBuffer(colors, nbrColors, colorsAttribLoc, offset);

}
void CubeObject::draw() {
	GLint positionLoc, colorLoc, normalLoc;

	vmath::mat4 projection = vmath::lookat(vmath::vec3(1.0, 1.0, 0.0),  // eye
		vmath::vec3(0.0, 0.0, 0.0),  // center 
		vmath::vec3(0.0, 0.0, 1.0)); // up
	vmath::mat4 viewing = vmath::rotate(45.0f, vmath::vec3(1.0f, 0.0f, 0.0f));
	viewing = vmath::scale(1.0f, 1.0f, 1.0f);
	vmath::mat4 modeling = vmath::scale(0.5f, 0.5f, 0.01f); // identity matrix...

	glUseProgram(programID);
	GLint projectionLocation = glGetUniformLocation(programID, "projectionTransform");
	if (projectionLocation == -1) {
		cerr << "Can't get location for projectionTransform uniform\n";
	}
	GLint viewingLocation = glGetUniformLocation(programID, "viewingTransform");
	if (viewingLocation == -1) {
		cerr << "Can't get location for viewingTransform uniform\n";
	}
	GLint modelingLocation = glGetUniformLocation(programID, "modelingTransform");
	if (modelingLocation == -1) {
		cerr << "Can't get location for modelingTransform uniform\n";
	}
	/*
	* Modified to skip transformations for debugging purposes.
	*/
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, projection);
	glUniformMatrix4fv(viewingLocation, 1, GL_FALSE, viewing);
	glUniformMatrix4fv(modelingLocation, 1, GL_FALSE, modeling);

	positionLoc = glGetAttribLocation(programID, "vPosition");
	if (positionLoc == -1) {
		cerr << "Can't get location for vPosition from shader\n";
	}
	colorLoc = glGetAttribLocation(programID, "vColor");
	if (colorLoc == -1) {
		cerr << "Can't get location for vColor from shader\n";
	}
	normalLoc = glGetAttribLocation(programID, "vNormal");
	if (normalLoc == -1) {
		cerr << "Can't get location for vNormal from shader\n";
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(4.0);
	glBindVertexArray(vaobjectID);
	glBindBuffer(GL_ARRAY_BUFFER, baobjectID);
	glDrawArrays(GL_TRIANGLES, 0, triangleCount * 3);
}

void CubeObject::draw(vmath::mat4 transform) {
	draw();
}