#version 330

// Sorties du shader
in vec3 vPosition_vs; // Position du sommet transformée dans l'espace View (vs)
in vec3 vNormal_vs; // Normale du sommet transformée dans l'espace View (vs)
in vec2 vTexCoords; // Coordonnées de texture du sommet

out vec4 fFragColor;

uniform sampler2D uTexture;

void main() {
    vec3 normalizedNormal = normalize(vNormal_vs);
    fFragColor = vec4(normalizedNormal, 1.0);

    /*vec4 texture1 = texture(uTexture, vTexCoords);
    fFragColor = vec4(texture1);*/
}