#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;

// Imports the current position from the Vertex Shader
in vec3 crntPos;
// Imports the normal from the Vertex Shader
in vec3 Normal;
// Imports the color from the Vertex Shader
in vec3 color;
// Imports the texture coordinates from the Vertex Shader
in vec2 texCoord;

/////// Try to implement spotlight is commented

// Gets the Texture Units from the main function
uniform sampler2D diffuse0; //texture
uniform sampler2D specular0;
//parameters to set w shader.set
// Gets the color of the light from the main function
uniform vec3 lightColor1;
uniform vec3 lightColor2;
// uniform vec3 spotColor;
// Gets the position of the light from the main function
uniform vec3 lightPosition1;
uniform vec3 lightPosition2;
//uniform vec3 spotPosition; 
// uniform vec3 spotDirection;
// uniform float spotCutOff;

// Gets the position of the camera from the main function
uniform vec3 camPos;

vec4 direcLight()
{
	vec3 lightVec1 = lightPosition1 - vec3(0); // lightDirection sun 1
	vec3 lightVec2 = lightPosition2 - vec3(0); // lightDirection sun 2

	// vec3 lightDirSpotMesh = (spotPosition - crntPos);

	// ambient lighting
	float ambient = 0.25f;

	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection1 = normalize(lightVec1);
	vec3 lightDirection2 = normalize(lightVec2);
	float diffuse1 = max(dot(normal, lightDirection1), 0.0f);
	float diffuse2 = max(dot(normal, lightDirection2), 0.0f);


	// specular lighting
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection1 = reflect(-lightDirection1, normal);
	vec3 reflectionDirection2 = reflect(-lightDirection2, normal);
	// vec3 reflectionDirectionSpot = reflect(-spotDirection, normal);
	float specAmount1 = pow(max(dot(viewDirection, reflectionDirection1), 0.0f), 16);
	float specAmount2 = pow(max(dot(viewDirection, reflectionDirection2), 0.0f), 16);
	// float specAmountSpot = pow(max(dot(viewDirection, reflectionDirectionSpot), 0.0f), 16);
	float specular1 = specAmount1 * specularLight;
	float specular2 = specAmount2 * specularLight;

	// float theta = dot((lightDirSpotMesh), normalize(spotDirection));

	// if (theta > spotCutOff){
	// 	return (texture(diffuse0, texCoord) * (diffuse1 + ambient) + 1.0 * specular1) * vec4(lightColor1,1.0) + 0.5 * vec4(spotColor, 1.0);
	// }
	return 0.8 * (texture(diffuse0, texCoord) * (diffuse1 + ambient) + specular1 ) * vec4(lightColor1, 1.0) +  (texture(diffuse0, texCoord) * (diffuse2 + ambient) + specular2) * vec4(lightColor2, 1.0);
}

void main() {
	// outputs final color
	FragColor = direcLight();
}