#version 410

uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelViewMatrix;
in vec2 texcoord;
in vec4 color;

uniform float alpha;
uniform sampler2DRect posData;
uniform vec3 tilt;

smooth out vec4 vColor;
out float vDepth;

void main() {

    vec3 pos = texture(posData, texcoord).xyz;
    vec3 p = pos * 100.0;

    float rx = radians(tilt.x);
    float ry = radians(tilt.y);
    float rz = radians(tilt.z);
    mat3 rotX = mat3(1.0, 0.0, 0.0,
                     0.0, cos(rx), -sin(rx),
                     0.0, sin(rx), cos(rx));
    mat3 rotY = mat3(cos(ry), 0.0, sin(ry),
                     0.0, 1.0, 0.0,
                     -sin(ry), 0.0, cos(ry));
    mat3 rotZ = mat3(cos(rz), -sin(rz), 0.0,
                     sin(rz), cos(rz), 0.0,
                     0.0, 0.0, 1.0);
    p = rotZ * rotY * rotX * p;

    vec4 mv = modelViewMatrix * vec4(p, 1.0);
    gl_Position = modelViewProjectionMatrix * vec4(p, 1.0);

    float size = mix(1.0, 4.0, clamp(alpha * 1.2 + 0.1, 0.0, 1.0));
    gl_PointSize = size * 500.0 / gl_Position.w;

    vDepth = mv.z / 120.0;
    vColor = color * (0.5 + alpha);
}
