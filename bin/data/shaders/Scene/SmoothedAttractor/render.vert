#version 410

uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelViewMatrix;
in vec2 texcoord;
in vec4 color;

uniform vec3 aPos;

in float alpha;
smooth out vec4 vColor;
out float vDepth;

uniform sampler2DRect posData;

void main() {

    vec3 pos = texture(posData, texcoord).xyz; // 0.0 - 1.0

    pos = (pos - aPos) * 200.0;

    gl_Position = modelViewProjectionMatrix * vec4(pos, 1.0);
    gl_PointSize = 500.0 / gl_Position.w;
    vDepth = (modelViewMatrix * vec4(pos, 1.0)).z / 100.0;
    vColor = color * alpha;
}
