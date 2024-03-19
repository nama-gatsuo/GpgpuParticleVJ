#version 410

uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelViewMatrix;
in vec2 texcoord;
in vec4 color;

out float vDepth;
out vec4 vColor;

uniform sampler2DRect posData;

void main() {

    vec3 pos = texture(posData, texcoord).xyz;
    
    vColor = color;
    gl_Position = modelViewProjectionMatrix * vec4(pos, 1.0);
    vDepth = (modelViewMatrix * vec4(pos, 1.0)).z / 100.0;
    gl_PointSize = 1000.0 / gl_Position.w;

}
