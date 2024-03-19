#version 400
precision highp float;

uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelViewMatrix;
in vec4 position; // random val in -1 ~ 1
in vec4 color;

out float vDepth;
out vec4 vColor;

uniform float alpha;
uniform float w0;
uniform mat4 mat0;
uniform float w1;
uniform mat4 mat1;
uniform float scale;

uniform float time;

float PI = 3.1415926535897932384626433832795;

float random(inout vec2 randomVec2) {

    float r = fract(sin(dot(randomVec2, vec2(12.9898, 78.233))) * 43758.5453);
    randomVec2[0] = randomVec2[1];
    randomVec2[1] = r;

    return r;

}

void main() {

    vec4 p = vec4(position.xyz, 1.0);

    vec2 randomVec2 = vec2(p.x, p.y);
    float r;

    for (int i = 0; i < 15; i++) {
        r = random(randomVec2);

        if (r < w0) {
            p = mat0 * p;
        } else if (r < w0 + w1) {
            p = mat1 * p;
        }

    }

    vColor = color * alpha;
    p = scale * p;

    gl_Position = modelViewProjectionMatrix * vec4(p.xyz, 1.0);
    vDepth = (modelViewMatrix * vec4(p.xyz, 1.0)).z / 100.0;
    gl_PointSize = 1500.0 / gl_Position.w;

}
