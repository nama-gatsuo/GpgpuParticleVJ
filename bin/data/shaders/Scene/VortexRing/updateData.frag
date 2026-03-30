#version 410

smooth in vec2 vTexCoord;

uniform sampler2DRect posData;
uniform sampler2DRect velData;

uniform float ringRadius;
uniform float ringThickness;
uniform float swirl;
uniform float pull;
uniform float noiseAmount;
uniform float jitterAmp;
uniform float deform;
uniform float twist;
uniform float phase;
uniform float dt;

layout (location = 0) out vec4 outputColor1;
layout (location = 1) out vec4 outputColor2;

float hash(vec3 p) {
    return fract(sin(dot(p, vec3(127.1, 311.7, 74.7))) * 43758.5453);
}

void main() {

    vec3 pos = texture(posData, vTexCoord).xyz;
    vec3 vel = texture(velData, vTexCoord).xyz;

    vec2 xz = pos.xz;
    float r = length(xz);
    vec2 dir = (r > 0.0001) ? (xz / r) : vec2(1.0, 0.0);

    float angle = atan(pos.z, pos.x);
    float id0 = hash(vec3(vTexCoord, 0.0));
    float id1 = hash(vec3(vTexCoord, 13.7));

    float angleWarp = angle + sin(angle * 3.0 + phase * 0.7 + id1 * 6.283) * deform;
    angleWarp += pos.y * twist;

    float wave = sin(phase + r * 0.03 + angleWarp * 2.7);
    float wobble = sin(angleWarp * 5.0 + phase * 1.3 + id0 * 6.283) * ringThickness * 0.38;
    float band = (id0 - 0.5) * ringThickness * 4.4;
    float targetR = ringRadius + band + wave * ringThickness * 0.25 + wobble;

    vec2 warpDir = vec2(cos(angleWarp), sin(angleWarp));
    vec2 radial = warpDir * (targetR - r);
    vec3 swirlDir = normalize(vec3(-warpDir.y, 0.0, warpDir.x));

    vec3 accel = vec3(radial.x, 0.0, radial.y) * pull + swirlDir * (swirl * (0.6 + 0.4 * sin(angleWarp * 3.0 + phase)));
    float targetY = (id1 - 0.5) * ringThickness * 3.6;
    accel.y += (targetY - pos.y) * 0.8;

    float n = hash(pos + vec3(phase));
    vec3 jitter = normalize(vec3(n - 0.5, fract(n * 7.3) - 0.5, fract(n * 13.1) - 0.5));
    float jitterPer = mix(0.2, 1.0, id1);
    accel += jitter * noiseAmount * jitterPer * jitterAmp;

    vel = vel * 0.96 + accel * dt;
    pos += vel * dt;

    outputColor1 = vec4(pos, 1.0);
    outputColor2 = vec4(vel, 1.0);
}
