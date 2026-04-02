#version 410

uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelViewMatrix;
in vec2 texcoord;

uniform sampler2DRect posData;
uniform float alpha;
uniform float worldScale;
uniform float numTypes;

smooth out vec4 vColor;
out float vDepth;

vec3 palette(int t) {
    if (t == 0) return vec3(1.00, 1.00, 1.00); // white
    if (t == 1) return vec3(1.00, 0.20, 0.20); // red
    if (t == 2) return vec3(1.00, 0.55, 0.55); // light red
    if (t == 3) return vec3(0.95, 0.95, 0.95); // near white
    if (t == 4) return vec3(1.00, 0.35, 0.35); // deep red
    return vec3(0.85, 0.85, 0.85);             // gray
}

void main() {

    vec4 p = texture(posData, texcoord);
    vec3 pos = p.xyz * worldScale;
    int typeId = int(clamp(floor(p.w + 0.5), 0.0, max(numTypes - 1.0, 0.0)));

    vec4 mv = modelViewMatrix * vec4(pos, 1.0);
    gl_Position = modelViewProjectionMatrix * vec4(pos, 1.0);
    gl_PointSize = (1200.0 + 450.0 * clamp(alpha, 0.0, 1.0)) / gl_Position.w;

    vec3 col = palette(typeId);
    vColor = vec4(col, alpha);
    vDepth = mv.z / 200.0;
}
