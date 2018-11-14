#version 330 core
//
// This is the initial version to do Lighting with the calculation in the
// fragment shader.  The basic form should stay the same for all the shaders
// with the shading in the fragment shader. 
//
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat3 NormalMatrix;

/*
 * Input variables -- attributes
 * vertexPosition, vertexNormal, and materialIndex.
 */
in vec3 vertexPosition;
in vec3 vertexNormal;

/*
 * Output Variables
 * The Color (superceded by the materialID ), the normal.
 */
out vec4 Color;
out vec3 Normal;
out vec4 Position;

/*
 * The main program.  I have a little more matrix multiplication here than the text does.  
 * It could be easily converted to the text's protocol of sending multiple combined 
 * matrices instead of entirely separate matrices. 
 */
void main () {	
	Color = vec4(1.0);
	mat3 normalTransform = transpose(inverse(NormalMatrix));
	Normal = normalize (vertexNormal * normalTransform);
	Position = ViewMatrix * ModelMatrix * vec4(vertexPosition, 1.0f);
	gl_Position = ProjectionMatrix * ViewMatrix *
		          ModelMatrix * vec4(vertexPosition, 1.0f);
}
