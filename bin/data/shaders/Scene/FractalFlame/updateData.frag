#version 410

smooth in vec2 vTexCoord;

uniform sampler2DRect posData;
uniform sampler2DRect colData;

uniform vec3 uM0[4];
uniform vec3 uM1[4];
uniform vec3 uM2[4];
uniform vec3 uT[4];
uniform float uW[4];
uniform float uVarSin[4];
uniform float uVarSwirl[4];
uniform float uVarSph[4];

uniform float phase;
uniform float react;
uniform float dt;

layout (location = 0) out vec4 outputColor1;
layout (location = 1) out vec4 outputColor2;

float hash(vec3 p) {
    return fract(sin(dot(p, vec3(127.1, 311.7, 74.7))) * 43758.5453);
}

vec3 applyMat(vec3 m0, vec3 m1, vec3 m2, vec3 v) {
    return vec3(dot(m0, v), dot(m1, v), dot(m2, v));
}

vec3 vSin(vec3 p) {
    return sin(p);
}

vec3 vSwirl(vec3 p) {
    float r2 = dot(p, p);
    return vec3(
        p.x * sin(r2) - p.y * cos(r2),
        p.x * cos(r2) + p.y * sin(r2),
        p.z * (0.6 + 0.4 * sin(r2))
    );
}

vec3 vSph(vec3 p) {
    float r2 = max(dot(p, p), 0.001);
    return p / r2;
}

vec3 palette(int idx) {
    if (idx == 0) return vec3(1.0, 0.3, 0.05);
    if (idx == 1) return vec3(0.1, 0.75, 1.0);
    if (idx == 2) return vec3(1.0, 0.1, 0.8);
    return vec3(0.4, 1.0, 0.2);
}

void main() {

    vec3 pos = texture(posData, vTexCoord).xyz;
    vec3 col = texture(colData, vTexCoord).xyz;

    // position-driven choice to avoid collapse to a single fixed point
    float r = hash(vec3(pos * 1.7 + vec3(phase * 0.08)));
    int idx = 0;
    float acc = 0.0;
    for (int i = 0; i < 4; i++) {
        acc += uW[i];
        if (r <= acc) { idx = i; break; }
    }

    vec3 p = applyMat(uM0[idx], uM1[idx], uM2[idx], pos) + uT[idx];

    float vs = uVarSin[idx];
    float vw = uVarSwirl[idx] * (0.6 + 0.8 * react);
    float vp = uVarSph[idx];
    float vlin = max(0.0, 1.0 - (vs + vw + vp));

    vec3 q = vec3(0.0);
    q += vlin * p;
    q += vs * vSin(p * (1.0 + react * 0.8));
    q += vw * vSwirl(p);
    q += vp * vSph(p);

    float warp = sin(phase * 0.7 + float(idx) * 2.1) * (0.6 + react);
    q = vec3(
        q.x + q.y * warp,
        q.y - q.z * warp,
        q.z + q.x * warp
    );

    // keep bounded to avoid divergence
    float len = length(q);
    float bound = 2.2;
    if (len > bound) {
        q *= bound / len;
    }
    q *= 0.85;
    q *= (0.96 + 0.04 * sin(phase * 0.3));
    vec3 targetPos = q;
    float smoothPos = 0.4 + react * 0.25;
    vec3 newPos = mix(pos, targetPos, smoothPos);
    //newPos *= 0.995;

    vec3 targetCol = palette(idx) * (0.4 + 0.9 * react);
    col = mix(col, targetCol, 0.08 + react * 0.15);

    outputColor1 = vec4(newPos, 1.0);
    outputColor2 = vec4(col, 1.0);
}
