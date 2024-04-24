#version 330

uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;

uniform vec3 uLightDir_vs;
uniform vec3 uLightIntensity;

in vec3 vPosition_vs;
in vec3 vNormal_vs;
in vec2 vTexCoords;

out vec4 fFragColor;

vec3 blinnPhong(vec3 fragPos, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(uLightDir_vs); // Direction incidente de la lumière
    vec3 halfVector = normalize(lightDir + normalize(-fragPos)); // Vecteur halfVector
    float diffuseFactor = max(dot(normal, lightDir), 0.0); // Produit scalaire entre la normale et la direction incidente
    float specularFactor = pow(max(dot(normal, halfVector), 0.0), uShininess); // Produit scalaire entre le vecteur halfVector et la normale
    vec3 diffuseColor = uLightIntensity * uKd * diffuseFactor; // Calcul de la couleur diffuse
    vec3 specularColor = uLightIntensity * uKs * specularFactor; // Calcul de la couleur glossy    
    return diffuseColor + specularColor; // Couleur finale en combinant diffuse et glossy
}

void main() {
    vec3 viewDir = normalize(-vPosition_vs);
    vec3 finalColor = blinnPhong(vPosition_vs, vNormal_vs, viewDir);
    fFragColor = vec4(finalColor, 1.0);
    // vec3 normalizedNormal = normalize(vNormal_vs);
    // fFragColor = vec4(normalizedNormal, 1.0);
}
