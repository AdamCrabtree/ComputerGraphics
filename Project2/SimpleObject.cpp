#include "SimpleObject.h"


/*
* This method is to add a subbuffer to the main buffer.  It is used to copy over data for normals, colors, etc...
* It can produce an error if a vaule is incorrect.
*
* Parameters:
*    data is a pointer to an array of float data to be added into the subbuffer
*    bufferSize is the size of data in bytes.
*    attribLoc is the attribute location to be used with this subdata
*    offset is the offset into the buffer for this subbuffer (in bytes)
*    count is the number of elements per data item.  It defaults to 4 but can be passed in.
*
*/
void SimpleObject::addSubBuffer(GLfloat *data, int bufferSize, int attribLoc, int &offset, int count) {
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

/*
* buildObject is a utility routine to build up an object.  It isn't complete -- I would still like to
* add texture information as a possible pass into this routine but right now, what we have is enough.
*
* Parameters:
*
*/
void SimpleObject::buildObject(GLfloat *vertices, int nbrVertices, int vertexAttribLoc,
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
void SimpleObject::draw() {
	GLint positionLoc, colorLoc, normalLoc;

	//vmath::mat4 projection = vmath::lookat( vmath::vec3(0.0, 1.0, 0.0),  // eye
	//										vmath::vec3(0.0, 0.0, 0.0),  // center 
	//										vmath::vec3(0.0, 0.0, 1.0)); // up

	vmath::mat4 projection = vmath::lookat(vmath::vec3(1.0, 1.0, 0.0),  // eye
		vmath::vec3(0.0, 0.0, 0.0),  // center 
		vmath::vec3(0.0, 0.0, 1.0)); // up

	vmath::mat4 viewing = vmath::rotate(90.0f, vmath::vec3(1.0f, 0.0f, 0.0f));
	viewing = vmath::scale(1.0f, 1.0f, 1.0f);
	vmath::mat4 modeling = vmath::scale(0.08f, 0.08f, 0.02f); // identity matrix...

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

void SimpleObject::draw(vmath::mat4 transform) {
	GLuint positionLoc, colorLoc, normalLoc;

	positionLoc = glGetAttribLocation(programID, "vPosition");
	colorLoc = glGetAttribLocation(programID, "vColor");
	normalLoc = glGetAttribLocation(programID, "vNormal");

	glBindBuffer(GL_ARRAY_BUFFER, baobjectID);
	glDrawArrays(GL_TRIANGLES, 0, triangleCount);
}
