#include <glew.h>
#include <freeglut.h>
#include <string>
#include <iostream>
#include <cstdlib>
#include "LoadShaders.h"
#include "vmath.h"
#include "vgl.h"
#include <map>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

#define BUFFER_OFFSET(x)  ((const void*) (x))
int rotationFlag = 1;
GLuint programID;
/*
* ID's for the vertex, element, and array buffer objects.
*/

GLuint vertexBufferObj0, elementBufferObj0, arrayBufferObj0;
GLuint vertexBufferObj1, elementBufferObj1, arrayBufferObj1;
/*
* Global variables
*   The location for the transformation and the current rotation
*   angle are set up as globals since multiple methods need to
*   access them.
*/
float rotationAngle;
bool elements;
int nbrTriangles0, nbrTriangles1, materialToUse = 0;

map<string, GLuint> locationMap;
vmath::mat4 projectionMatrix = vmath::scale(1.0f);
vmath::mat4 viewingMatrix = vmath::scale(1.0f);
float deltax;
float deltay;
float deltaz;
// Prototypes
GLuint buildProgram(string vertexShaderName, string fragmentShaderName);
int glutStartUp(int & argCount, char *argValues[],
	string windowTitle = "No Title", int width = 500, int height = 500);
void setAttributes(float lineWidth = 1.0, GLenum face = GL_FRONT_AND_BACK,
	GLenum fill = GL_FILL);
void buildDeerAndCube();
void getLocations();
void init(string vertexShader, string fragmentShader);
vmath::mat4 ortho(float znear, float zfar, float left, float right, float bottom, float top);
void readfile(string filename, vector<float>& vertices, vector<unsigned int>& indices);
void readfile2(string filename, vector<float>& vertices, vector<float>& normals);
void readfile3(string filename, vector<float>& vertices, vector<float>& normals, vector<float>& textures);
/*
* Routine to encapsulate some of the startup routines for GLUT.  It returns the window ID of the
* single window that is created.
*/
int glutStartUp(int & argCount, char *argValues[], string title, int width, int height) {
	int windowID;
	GLenum glewErrCode;

	glutInit(&argCount, argValues);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(width, height);
	windowID = glutCreateWindow(title.c_str());

	glutInitContextVersion(3, 1);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glewErrCode = glewInit();
	if (glewErrCode != GLEW_OK) {
		cerr << "Glew init failed " << glewErrCode << endl;
		exit(EXIT_FAILURE);
	}
	return windowID;
}


/*
* Use the author's routines to build the program and return the program ID.
*/
GLuint buildProgram(string vertexShaderName, string fragmentShaderName) {

	/*
	*  Use the Books code to load in the shaders.
	*/
	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, vertexShaderName.c_str() },
	{ GL_FRAGMENT_SHADER, fragmentShaderName.c_str() },
	{ GL_NONE, NULL }
	};
	GLuint program = LoadShaders(shaders);
	if (program == 0) {
		cerr << "GLSL Program didn't load.  Error \n" << endl
			<< "Vertex Shader = " << vertexShaderName << endl
			<< "Fragment Shader = " << fragmentShaderName << endl;
	}
	glUseProgram(program);
	return program;
}

/*
* Set up the clear color, lineWidth, and the fill type for the display.
*/
void setAttributes(float lineWidth, GLenum face, GLenum fill) {
	/*
	* I'm using wide lines so that they are easier to see on the screen.
	* In addition, this version fills in the polygons rather than leaving it
	* as lines.
	*/
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glLineWidth(lineWidth);
	glPolygonMode(face, fill);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
}

/*
* read and/or build the objects to be displayed.  Also sets up attributes that are
* vertex related.
*
* This version will read in from the file deerraw.obj with a limited format for the
* file.
*/

void buildDeerAndCube() {

	GLfloat colors[] = { 1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f
	};

	vector<float> vertices;
	vector<unsigned int> indices;
	readfile("DeerRaw.obj", vertices, indices);

	glGenBuffers(1, &elementBufferObj0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObj0);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
		indices.data(), GL_STATIC_DRAW);

	glGenVertexArrays(1, &vertexBufferObj0);
	glBindVertexArray(vertexBufferObj0);

	// Alternately...
	// GLuint   vaoID;
	// glGenVertexArrays(1, &vaoID);
	// glBindVertexArray(vaoID);
	//

	/*
	* Test code for internal object.
	*/
	nbrTriangles0 = indices.size() / 3;
	glGenBuffers(1, &arrayBufferObj0);
	glBindBuffer(GL_ARRAY_BUFFER, arrayBufferObj0);
	glBufferData(GL_ARRAY_BUFFER,
		vertices.size() * sizeof(float),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float),
		vertices.data());
	/*
	* Set up variables into the shader programs (Note:  We need the
	* shaders loaded and built into a program before we do this)
	*/
	GLuint vPosition = glGetAttribLocation(programID, "vertexPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));

	vector <float>model2vertices, model2normals, model2textures;

	readfile2("cube.obj", model2vertices, model2normals);
	glGenVertexArrays(1, &vertexBufferObj0);
	glBindVertexArray(vertexBufferObj0);
	nbrTriangles0 = model2vertices.size() / 9;
	glGenBuffers(1, &arrayBufferObj0);
	glBindBuffer(GL_ARRAY_BUFFER, arrayBufferObj0);
	glBufferData(GL_ARRAY_BUFFER,
		model2vertices.size() * sizeof(float) + model2normals.size() * sizeof(float),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, model2vertices.size() * sizeof(float),
		model2vertices.data());
	glBufferSubData(GL_ARRAY_BUFFER, model2vertices.size() * sizeof(float),
		model2normals.size() * sizeof(float), model2normals.data());
	vPosition = glGetAttribLocation(programID, "vertexPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));

	GLuint vNormal = glGetAttribLocation(programID, "vertexNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(model2vertices.size() * sizeof(float)));
}

void BuildDeer() {

	GLfloat colors[] = { 1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f
	};

	vector<float> vertices;
	vector<unsigned int> indices;
	readfile("DeerRaw.obj", vertices, indices);

	glGenBuffers(1, &elementBufferObj1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObj1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
		indices.data(), GL_STATIC_DRAW);

	glGenVertexArrays(1, &vertexBufferObj1);
	glBindVertexArray(vertexBufferObj1);

	// Alternately...
	// GLuint   vaoID;
	// glGenVertexArrays(1, &vaoID);
	// glBindVertexArray(vaoID);
	//

	/*
	* Test code for internal object.
	*/
	nbrTriangles1 = indices.size() / 3;
	glGenBuffers(1, &arrayBufferObj1);
	glBindBuffer(GL_ARRAY_BUFFER, arrayBufferObj1);
	glBufferData(GL_ARRAY_BUFFER,
		vertices.size() * sizeof(float),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float),
		vertices.data());
	/*
	* Set up variables into the shader programs (Note:  We need the
	* shaders loaded and built into a program before we do this)
	*/
	GLuint vPosition = glGetAttribLocation(programID, "vertexPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));

	vector <float>model2vertices, model2normals;
	GLuint vNormal = glGetAttribLocation(programID, "vertexNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(model2vertices.size() * sizeof(float)));
}

/*
* This fills in the locations of most of the uniform variables for the program.
* there are better ways of handling this but this is good in going directly from
* what we had.
*
* Revised to get the locations and names of the uniforms from OpenGL.  These
* are then stored in a map so that we can look up a uniform by name when we
* need to use it.  The map is still global but it is a little neater than the
* version that used all the locations.  The locations are still there right now
* in case that is more useful for you.
*
*/

void getLocations() {
	/*
	* Find out how many uniforms there are and go out there and get them from the
	* shader program.  The locations for each uniform are stored in a global -- locationMap --
	* for later retrieval.
	*/
	GLint numberBlocks;
	char uniformName[1024];
	int nameLength;
	GLint size;
	GLenum type;
	glGetProgramiv(programID, GL_ACTIVE_UNIFORMS, &numberBlocks);
	for (int blockIndex = 0; blockIndex < numberBlocks; blockIndex++) {
		glGetActiveUniform(programID, blockIndex, 1024, &nameLength, &size, &type, uniformName);
		cout << uniformName << endl;
		locationMap[string(uniformName)] = blockIndex;
	}
}

void init(string vertexShader, string fragmentShader) {

	setAttributes(1.0f, GL_FRONT_AND_BACK, GL_FILL);

	programID = buildProgram(vertexShader, fragmentShader);

	BuildDeer();

	getLocations();

}

/*
* Timer routine -- when called it increments the angle by 1 degree,
* tells Glut to redraw the screen, and sets a timer for 1/30 of a
* second later.
*/
void timer(int value) {
	if (rotationFlag == 0) {
		glutPostRedisplay();
		glutTimerFunc(1000 / 30, timer, 1);
	}
	else {
		rotationAngle += 1.0f;
		if (rotationAngle >= 360.0f)
			rotationAngle -= 360.0;
		glutPostRedisplay();
		glutTimerFunc(1000 / 30, timer, 1);
	}
}

/*
* The display routine is basically unchanged at this point.
*/
void display() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// needed
	vmath::mat4 currentMatrix = vmath::translate(deltax, deltay, deltaz);
	vmath::mat4 myMatrix = vmath::translate(vmath::vec3(10.0f, 0.0f, 0.0f));
	float mycolor[] = { 0.5, 0.1, 0.1, 1.0 };
	glUniformMatrix4fv(locationMap["ModelMatrix"], 1, GL_TRUE,
		currentMatrix);
	vmath::mat4 normalMatrix4 =  vmath::scale(0.25f) * vmath::rotate(rotationAngle, vmath::vec3(0.0, 1.0, 0.0));
	//vmath::translate(deltax, deltay, deltaz)
	projectionMatrix = ortho(-100.0f, 100.0f, -2.0f, 2.0f, -2.0f, 2.0f);
	projectionMatrix = vmath::scale(1.0f);
	viewingMatrix = vmath::scale(1.0f);
	glUniformMatrix4fv(locationMap["ProjectionMatrix"], 1, GL_TRUE, projectionMatrix);
	glUniformMatrix4fv(locationMap["ViewMatrix"], 1, GL_TRUE, viewingMatrix);
	glUniform4fv(locationMap["deerColor"], 1, mycolor);
	float normalMatrix[3][3] = { {100,100}, {100,300}, {400,400} };
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			normalMatrix[i][j] = normalMatrix4[i][j];
		}
	}
	glUniformMatrix3fv(locationMap["NormalMatrix"], 1, GL_TRUE, (float *)normalMatrix);

	glBindVertexArray(vertexBufferObj1);
	glBindBuffer(GL_ARRAY_BUFFER, arrayBufferObj1);
	glDrawArrays(GL_TRIANGLES, 0, nbrTriangles1 * 3);


	glutSwapBuffers();
}

/*
* Handle keypresses -- only one recognized in this version is q for quit.
*/

void keypress(unsigned char key, int x, int y) {
	switch (key) {
	case 'r':
		rotationFlag = rotationFlag ^ 1;
		break;
	case 'q':
	case 'Q':
		exit(0);
		break;
	case 'o':
	case'O':
		projectionMatrix = ortho(-100.0f, 100.0f, -3.0f, 3.0f, -3.0f, 3.0f);
		break;


	case 'p':
	case 'P':
		projectionMatrix = vmath::scale(1.0f);
		projectionMatrix[0][0] = 0.1 / (0.5 * 0.1); // width
		projectionMatrix[1][1] = 0.1 / (0.5 * 0.1); // height
		projectionMatrix[2][2] = -1.0*(10.0 + 0.1) / (0.5 * (10.0 - 0.1)); // depth
		projectionMatrix[2][3] = -1.0 * 10.0 * 0.1 / (10.0 - 0.1);
		projectionMatrix[3][2] = -1.0;
		projectionMatrix = vmath::lookat(vmath::vec3(0.0, 0.0, -10.0), vmath::vec3(0.0, 0.0, 0.0), vmath::vec3(0.0, 1.0, 0.0));
		break;
	default:
		// do nothing....
		break;
	}
}

void specialKeypress(int keycode, int x, int y) {
	switch (keycode) {
	case GLUT_KEY_UP:
		deltay++;
		break;
	case GLUT_KEY_DOWN:
		deltay--;
		break;
	case GLUT_KEY_LEFT:
		deltax++;
		break;
	case GLUT_KEY_RIGHT:
		deltay--;
		break;
	default:
		break;
	}
}
/*
* Main program with calls for many of the helper routines.
*/
int main(int argCount, char *argValues[]) {

	glutStartUp(argCount, argValues, "My Test of New Routines");
	init("vshader0.vert", "fshader2.frag");
	glutDisplayFunc(display);
	glutTimerFunc(1000 / 10, timer, 1);
	glutKeyboardFunc(keypress);
	glutSpecialFunc(specialKeypress);
	glutMainLoop();
}