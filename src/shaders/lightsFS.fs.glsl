#version 330

in vec3 vPosition_vs;
in vec3 vNormal_vs;
in vec2 vTexCoords;

uniform vec3 uDiffuse;
uniform vec3 uSpecular;
uniform float uShininess;

uniform vec3 uLight1Position;
uniform vec3 uLight1Intensity;
uniform vec3 uLight2Direction;
uniform vec3 uLight2Intensity;

uniform bool uGUIPointLight;
uniform bool uGUIDirectionalLight;
uniform bool uGUIAmbientLight;

out vec4 fFragColor;

// uniform float uOpacity;
uniform sampler2D uTexture;

float computeAttenuation(float distance, float constant, float linear, float quadratic) {
    return 1.0 / (constant + linear * distance + quadratic * (distance * distance));
}

vec3 blinnPhongPointLight() {
    vec3 Wo = normalize(-vPosition_vs);
    vec3 Wi = normalize(uLight1Position - vPosition_vs);

    float distance = length(uLight1Position - vPosition_vs);
    float attenuation = computeAttenuation(distance, 1.0, 0.14, 0.07);

    vec3 Li = uLight1Intensity * attenuation;
    vec3 halfVector = normalize(Wo + Wi);

    return Li * (uDiffuse * dot(Wi, vNormal_vs)) + uSpecular * pow(dot(halfVector, vNormal_vs), uShininess);
}

vec3 blinnPhongDirectionalLight() {
    vec3 Wo = normalize(-vPosition_vs);
    vec3 Wi = normalize(uLight2Direction);

    vec3 halfVector = normalize(Wo + Wi);

    return uLight2Intensity * (uDiffuse * dot(Wi, vNormal_vs)) + uSpecular * pow(max(dot(halfVector, vNormal_vs), 0), uShininess);
}

vec3 ambientLight() {
    vec3 color = vec3(0.7, 0.7, 0.5);
    float intensity = 0.6;
    return color * intensity;
}

void main() {
    vec3 vColor = texture(uTexture, vTexCoords).xyz;

    // Lights
    vec3 pointLightValue = vec3(0);
    if (uGUIPointLight) {
        pointLightValue = vec3(clamp(blinnPhongPointLight(), 0, 1));
    }
    vec3 directionalLightValue = vec3(0);
    if (uGUIDirectionalLight) {
        directionalLightValue = vec3(clamp(blinnPhongDirectionalLight(), 0, 1));
    }

    vec3 ambientLightValue = vec3(0);
    if (uGUIAmbientLight) {
        ambientLightValue = vec3(clamp(ambientLight(), 0, 1));
    }

    if (uGUIPointLight || uGUIDirectionalLight || uGUIAmbientLight) {
        vec3 lightSum = vec3(clamp(pointLightValue + directionalLightValue + ambientLightValue, 0, 1));
        vColor *= lightSum;
    }

    fFragColor = vec4(vColor.rgb, 1);
}
