#version 330

in vec3 vPosition_vs;
in vec3 vNormal_vs;
in vec2 vTexCoords;

out vec4 fFragColor;

uniform sampler2D uText;

uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;
uniform vec3 uLightPos_vs;
uniform vec3 uLightIntensity;
uniform vec3 uLightPos2_vs;
uniform vec3 uLightIntensity2;

vec3 blinnPhong(vec3 uLightPos_vs, vec3 uLightIntensity){
    float d = distance(vPosition_vs, uLightPos_vs);
    vec3 wi= normalize(uLightPos_vs-vPosition_vs);
    vec3 Li= (uLightIntensity/(d*d));
    vec3 w0 = (normalize(-vPosition_vs));
    vec3 N = vNormal_vs;
    vec3 halfVector = (w0+wi)/2.f;
    return Li*(uKd*(dot(wi, N))+uKs*pow(dot(halfVector,N),uShininess));
}


void main() {    

    vec3 light1 = blinnPhong(uLightPos_vs, uLightIntensity);
    vec3 color = light1;
    
    vec3 light2 = blinnPhong(uLightPos2_vs, uLightIntensity2);
    color += light2;
    
    vec4 texture = texture(uText, vTexCoords);
    
    fFragColor = vec4(color, 1.0)*texture;	
}