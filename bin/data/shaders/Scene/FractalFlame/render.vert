#version 410

uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelViewMatrix;
in vec2 texcoord;
in vec4 color;

uniform float alpha;
uniform sampler2DRect posData;
uniform sampler2DRect colData;

smooth out vec4 vColor;
out float vDepth;

void main() {

    vec3 pos = texture(posData, texcoord).xyz;
    vec3 colp = texture(colData, texcoord).xyz;

    vec3 p = pos * 260.0;

    vec4 mv = modelViewMatrix * vec4(p, 1.0);
    gl_Position = modelViewProjectionMatrix * vec4(p, 1.0);

    float size = mix(0.6, 3.5, clamp(alpha, 0.0, 1.0));
    gl_PointSize = size * 750.0 / gl_Position.w;

    vDepth = mv.z / 120.0;
    vColor = vec4(colp, 1.0) * alpha;
}
