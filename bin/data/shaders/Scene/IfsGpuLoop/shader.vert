#version 400

uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelViewMatrix;

in vec4 position;
in vec4 color;

out vec4 vColor;
out float vDepth;

uniform float alpha;
uniform float worldScale;
uniform float tBase;
uniform float tSpread;
uniform float phaseSpeed;
uniform int numClasses;
uniform int loopCount;
uniform float time;

vec3 palette(float x) {
    vec3 a = vec3(0.55, 0.55, 0.65);
    vec3 b = vec3(0.55, 0.45, 0.35);
    vec3 c = vec3(1.00, 1.10, 1.20);
    vec3 d = vec3(0.00, 0.18, 0.42);
    return a + b * cos(6.28318 * (c * x + d));
}

float hash11(float p) {
    return fract(sin(p * 127.1) * 43758.5453123);
}

float hash31(vec3 p) {
    return fract(sin(dot(p, vec3(127.1, 311.7, 74.7))) * 43758.5453123);
}

void main() {

    vec3 p = position.xyz;

    int classId = clamp(int(floor(color.r * float(max(numClasses - 1, 1)) + 0.5)), 0, max(numClasses - 1, 0));
    float cls = float(classId);
    float seed = color.g;

    float R = 6.28318530718 / float(max(numClasses, 1));
    float clsPhase = R * cls;
    float phase = time * phaseSpeed;

    float tClass = phase;

    const int maxLoop = 64;
    for (int j = 0; j < maxLoop; j++) {
        if (j >= loopCount) break;

        float jf = float(j);
        float drift = hash31(vec3(seed * 13.7, jf * 0.73, cls * 0.19)) - 0.5;
        float wJitter = (hash11(seed * 91.0 + jf * 1.11) - 0.5) * 0.4;

        float u = sin(p.x + drift * 0.2) + sin(clsPhase + p.y);
        float v = cos(p.y - drift * 0.2) + cos(clsPhase + p.z);
        float w = sin(p.z + wJitter) + sin(clsPhase + p.x);

        p = vec3(u + tClass, v + tClass, w);
    }

    vec3 pos = p * worldScale;
    vec4 mv = modelViewMatrix * vec4(pos, 1.0);
    gl_Position = modelViewProjectionMatrix * vec4(pos, 1.0);
    gl_PointSize = 1000.0 / gl_Position.w;

    float classNorm = cls / max(float(numClasses - 1), 1.0);
    vec3 col = palette(classNorm) * mix(0.55, 1.25, color.b);
    vColor = vec4(col, alpha);
    vDepth = mv.z / 190.0;
}
