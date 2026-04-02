#version 410

smooth in vec2 vTexCoord;

uniform sampler2DRect posData;
uniform sampler2DRect velData;

uniform float dt;
uniform float phase;
uniform int numTypes;
uniform float interactionRadius;
uniform float searchRange;
uniform float forceScale;
uniform float baseRepulsion;
uniform float boundRadius;
uniform float centerPull;
uniform float noiseAmount;
uniform vec2 simRes;
uniform float interaction[36];

layout (location = 0) out vec4 outputColor1;
layout (location = 1) out vec4 outputColor2;

float hash1(vec3 p) {
    return fract(sin(dot(p, vec3(127.1, 311.7, 74.7))) * 43758.5453123);
}

vec2 sampleUv(float i, vec2 uv) {
    float ang = fract(sin(i * 91.73 + phase * 0.53 + uv.y * 0.013) * 43758.5453) * 6.2831853;
    float radJitter = fract(sin(i * 17.37 + phase * 0.81 + uv.x * 0.017) * 24634.6345);
    float rad = (0.1 + 0.9 * radJitter) * max(searchRange, 1.0);
    vec2 p = uv + vec2(cos(ang), sin(ang)) * rad;
    return clamp(p, vec2(0.5), simRes - vec2(0.5));
}

float kernel(float q) {
    float repulse = 1.0 - smoothstep(0.0, 0.22, q);
    float attract = smoothstep(0.18, 0.58, q) * (1.0 - smoothstep(0.58, 1.0, q));
    return attract * 2.05 - repulse * 1.55;
}

void main() {

    vec4 posTex = texture(posData, vTexCoord);
    vec4 velTex = texture(velData, vTexCoord);
    vec3 pos = posTex.xyz;
    vec3 vel = velTex.xyz;

    int selfType = clamp(int(floor(posTex.w + 0.5)), 0, numTypes - 1);
    float damping = clamp(velTex.w, 0.85, 0.999);

    vec3 force = vec3(0.0);
    const int sampleCount = 128;
    for (int i = 0; i < sampleCount; i++) {
        vec2 uv = sampleUv(float(i), vTexCoord);
        vec4 other = texture(posData, uv);
        vec3 d = other.xyz - pos;
        float dist = length(d);
        if (dist > 0.0001 && dist < interactionRadius) {
            float q = dist / interactionRadius;
            int otherType = clamp(int(floor(other.w + 0.5)), 0, numTypes - 1);
            float rel = interaction[selfType * numTypes + otherType];
            force += (d / dist) * (kernel(q) * rel * 1.25);
            float keepDist = interactionRadius * 0.26;
            float rep = 1.0 - smoothstep(0.0, keepDist, dist);
            force -= (d / dist) * (rep * baseRepulsion);
        }
    }

    float n = hash1(pos + vec3(phase * 0.7));
    vec3 jitter = normalize(vec3(n - 0.5, fract(n * 13.17) - 0.5, fract(n * 29.31) - 0.5));
    force += jitter * noiseAmount * 1.0;

    // Avoid staying in the center: repel near origin and add a slight swirl flow.
    float rCenter = length(pos);
    float avoidZone = interactionRadius * 0.9;
    float avoid = 1.0 - smoothstep(0.0, avoidZone, rCenter);
    vec3 outward = (rCenter > 1e-5) ? (pos / rCenter) : vec3(1.0, 0.0, 0.0);
    vec3 swirl = normalize(vec3(-pos.z, 0.0, pos.x) + vec3(1e-5));
    force += outward * (centerPull * avoid);
    force += swirl * (centerPull * 0.35 * avoid);

    vec3 accel = force * forceScale;
    // Soften burst acceleration while keeping relative dynamics.
    float accelMag = length(accel);
    float softLimit = 1.2;
    accel *= softLimit / (softLimit + accelMag);

    vel = vel * damping + accel * (dt * 0.8);
    pos += vel * dt;

    float r = length(pos);
    if (r > boundRadius) {
        vec3 normal = pos / max(r, 1e-5);
        pos = normal * boundRadius;
        float outVel = dot(vel, normal);
        if (outVel > 0.0) {
            vel -= normal * outVel * 1.85;
        }
        vel *= 0.86;
    }

    outputColor1 = vec4(pos, posTex.w);
    outputColor2 = vec4(vel, damping);
}
