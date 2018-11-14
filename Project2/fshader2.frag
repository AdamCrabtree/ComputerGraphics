#version 330 core

/*
 * The fragment shader for version 0.  Here are all of our structures
 * and most of our calculations.  
 */

 struct LightProperties {
	bool	isEnabled;		// on or off
	bool	isLocal;		// directional or point source
	bool	isSpot;			// spotlight
	vec3	ambient;
	vec3	diffuse;
	vec3	specular;
	vec3	position;		
	vec3	halfVector;
	vec3	coneDirection;
	float	spotCosCutoff;
	float	spotExponent;
	float	constantAttenuation;
	float	linearAttenuation;
	float	quadraticAttenuation;
	};

struct MaterialProperties {
	vec3	emission;
	vec3	ambient;
	vec3	diffuse;
	vec3	specular;
	float	shininess;
};

const int MaxLights = 10;

uniform LightProperties Lights [MaxLights];

const int MaxProperties = 20;

uniform MaterialProperties Materials[MaxProperties];

uniform vec3 eyeDirection = { 0.0f, 0.0f, 3.0f};
uniform int  materialIndex = 0;

in vec4 Color;
in vec3 Normal;
in vec4 Position;


out vec4 FragColor;

void main() {
	LightProperties testLights[1];
	MaterialProperties testMaterials[1];

	testLights[0].isEnabled = true;
	testLights[0].isLocal = true;
	testLights[0].isSpot = false;
	testLights[0].ambient = vec3(0.25f);
	testLights[0].diffuse = vec3(0.5f);
	testLights[0].specular = vec3(0.0f);
	testLights[0].position = vec3(0.0f, 3.0f, 3.0f);

	testMaterials[0].ambient   = vec3(0.5f, 0.0f, 0.5f);
	testMaterials[0].diffuse   = vec3(1.0f, 1.0f, 1.0f);
	testMaterials[0].specular  = vec3(1.0f, 1.0f, 1.0f);
	testMaterials[0].shininess = 40.0f;
	testMaterials[0].emission  = vec3(0.0f);

	// Initial lighting level...
	vec3 scatteredLight = vec3(0.0f);
	vec3 reflectedLight = vec3(0.0f);
	vec3 halfVector, lightDirection;
	float diffuse, specular;

	for (int lightNbr = 0; lightNbr < 1; lightNbr++) {
		if (testLights[lightNbr].isEnabled) {
			scatteredLight += clamp(testLights[0].ambient * testMaterials[0].ambient,
								   0.0f, 1.0f);
		}

		if (testLights[lightNbr].isLocal && testLights[lightNbr].isEnabled) {
			lightDirection = testLights[lightNbr].position-vec3(Position);
			float lightDistance = length(lightDirection);
			lightDirection = normalize(lightDirection / lightDistance);
			halfVector = normalize(lightDirection + normalize(eyeDirection));
			diffuse = max(0.0f,dot(Normal, lightDirection));
			specular = max(0.0f, dot(Normal, halfVector));
			if (diffuse <= 0.0f) {
				specular = 0.0f;
				diffuse  = 0.0f;
			} else {
				specular = pow (specular, testMaterials[0].shininess);
			}
			if (dot(normalize(eyeDirection),Normal) < 0) { //faces away from me
				specular = 0.0f;
				diffuse = 0.0f;
			}
				reflectedLight += testLights[lightNbr].diffuse * testMaterials[0].diffuse * diffuse + 
								  testLights[lightNbr].specular * testMaterials[0].specular * specular;
		}
	}
	
	FragColor = clamp (vec4(scatteredLight + reflectedLight, vec3(1.0f)), 0.0f, 1.0f);
}