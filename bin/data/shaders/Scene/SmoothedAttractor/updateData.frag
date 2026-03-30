#version 410

smooth in vec2 vTexCoord;

uniform sampler2DRect posData;
uniform sampler2DRect velData;

uniform vec3 aPos;
uniform float ur;
uniform float uf; // target dead-zone radius
uniform float dt;

layout (location = 0) out vec4 outputColor1;
layout (location = 1) out vec4 outputColor2;

void main(){

    vec3 pos = texture(posData, vTexCoord).xyz; // 0.0 ~ 1.0

    vec4 velTex = texture(velData, vTexCoord); // 0.0 ~ 1.0
    vec3 vel = velTex.xyz;
    // Per-particle damping (small, fixed scale)
    float damp = clamp(velTex.w * 0.05, 0.0, 0.999);

    vec3 v = normalize(aPos - pos) * ur;
    vec3 w = normalize(vel + v);

    vel = w * (1.0 - damp);
    pos = pos + dt * (vel + v);

    outputColor1 = vec4(pos, 1.0); //position
    outputColor2 = vec4(vel, velTex.w); //velocity + per-particle damping
}
