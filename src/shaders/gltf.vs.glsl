#version 330 core

// Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Normals (not necessarily normalized)
layout (location = 1) in vec3 aNormal;
// Colors
layout (location = 2) in vec3 aColor;
// Texture Coordinates
layout (location = 3) in vec2 aTex;


// Outputs the current position for the Fragment Shader
out vec3 crntPos;
// Outputs the normal for the Fragment Shader
out vec3 Normal;
// Outputs the color for the Fragment Shader
out vec3 color;
// Outputs the texture coordinates to the Fragment Shader
out vec2 texCoord;

// uniform mat4 model;
//parameters to set w shader.set
uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

// Imports the transformation matrices
uniform mat4 vmodel;
uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 scale;

void main()
{

	// calculates current position
	crntPos = vec3(vmodel * translation * -rotation * scale * vec4(aPos, 1.0f));

	// Assigns the normal from the Vertex Data to "Normal"
	Normal = mat3(transpose(inverse(model))) * aNormal;
	// Assigns the colors from the Vertex Data to "color"
	color = aColor;
	// Assigns the texture coordinates from the Vertex Data to "texCoord"
	texCoord = mat2(0.0, -1.0, 1.0, 0.0) * aTex;

	gl_Position = projection * view * model * vec4(crntPos, 1.0);
	crntPos = gl_Position.xyz;
	
}