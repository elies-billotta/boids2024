#version 330

in vec3 vPosition_vs;
in vec3 vNormal_vs;
in vec2 vTexCoords;

vec2 fCorrectCoords;

out vec4 fFragColor;

uniform sampler2D uTexture;

void main() {

    fCorrectCoords = vec2(vTexCoords.x, 1-vTexCoords.y);
	fFragColor = texture(uTexture, fCorrectCoords);
}