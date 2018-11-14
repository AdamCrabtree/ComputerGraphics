#version 330 core
/*
 * This is the project 3 shader file for the vertices.  It hopefully contains
 * all the pieces we are going to need for many of our projects.  It is 
 * Lots of uniform variables -- mostly dealing with color and 
 * transformations.
 */
uniform mat4 modelingTransform = {{1.0, 0.0, 0.0, 0.0},
                                  {0.0, 1.0, 0.0, 0.0},
								  {0.0, 0.0, 1.0, 0.0},
								  {0.0, 0.0, 0.0, 1.0}};
uniform mat4 projectionTransform = {{1.0, 0.0, 0.0, 0.0}, 
                                    {0.0, 1.0, 0.0, 0.0}, 
									{0.0, 0.0, 1.0, 0.0}, 
									{0.0, 0.0, 0.0, 1.0}};
uniform vec3 eyePosition = { 0.0, 0.0, 10.0};
uniform int materialID = 0;

in vec4 vPosition;
in vec4 vColor;
out vec4 passedColor;
void main() {
	gl_Position = modelingTransform * vPosition/vPosition.w;
	passedColor = vColor;

	// do the projection thing now...
	gl_Position = projectionTransform * modelingTransform * vPosition/vPosition.w;

}