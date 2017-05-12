#version 410

smooth in vec2 vTexCoord;

uniform sampler2DRect posData;
uniform sampler2DRect velData;

uniform float GM;
uniform float dt;
uniform float time;
uniform float massRatio;

uniform vec3 p0;
uniform vec3 p1;

layout (location = 0) out vec4 outputColor1;
layout (location = 1) out vec4 outputColor2;

void main(){

    vec4 pos = texture(posData, vTexCoord);
    vec3 vel = texture(velData, vTexCoord).xyz;
    
    float d0 = distance(pos.xyz, p0);
    float d1 = distance(pos.xyz, p1);
    
    if (d0 > 5.0 && d1 > 5.0) {
        vec3 acce;
        acce = normalize(p0 - pos.xyz) * GM * massRatio / d0;
        acce += normalize(p1 - pos.xyz) * GM / d1;
        vel.xyz += acce * dt;
        
        pos.xyz += vel.xyz * dt;
    }
    
    
    outputColor1 = pos; //position
    outputColor2 = vec4(vel, 1.0); //velocity
}
