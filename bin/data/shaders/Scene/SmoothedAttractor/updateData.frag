#version 410

smooth in vec2 vTexCoord;

uniform sampler2DRect posData;
uniform sampler2DRect velData;

uniform vec3 aPos;
uniform float ur;
uniform float dt;

layout (location = 0) out vec4 outputColor1;
layout (location = 1) out vec4 outputColor2;

void main(){

    vec3 pos = texture(posData, vTexCoord).xyz; // 0.0 ~ 1.0
    //pos = pos * 2.0 - vec3(1.0); // -1.0 ~ 1.0

    vec3 vel = texture(velData, vTexCoord).xyz; // 0.0 ~ 1.0
    //vel = vel * 2.0 - vec3(1.0);  // -1.0 ~ 1.0

    vec3 v = normalize(aPos - pos) * ur;
    vec3 w = normalize(vel + v) * 1.0;

    pos = pos + dt * (w + v);
    vel = w;
    
    //pos = 0.5 * (pos + vec3(1.0));
    //vel = 0.5 * (vel + vec3(1.0));

    outputColor1 = vec4(pos, 1.0); //position
    outputColor2 = vec4(vel, 1.0); //velocity
}
