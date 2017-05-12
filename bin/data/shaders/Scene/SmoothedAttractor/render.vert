#version 410

uniform mat4 modelViewProjectionMatrix;
in vec2 texcoord;
in vec4 color;

in float alpha;
smooth out vec4 vColor;

uniform sampler2DRect posData;

void main() {

    vec3 pos = texture(posData, texcoord).xyz; // 0.0 - 1.0

    pos = pos * 200.0 - vec3(100.0);

    gl_Position = modelViewProjectionMatrix * vec4(pos, 1.0);
    gl_PointSize = 1000.0 / gl_Position.w;
    
    vColor = color;
}
