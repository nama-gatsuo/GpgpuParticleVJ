#version 420

layout (binding = 0) uniform sampler2DRect tex;
uniform float threshold;
in vec2 vTexCoord;
out vec4 outputColor;

void main() {
    vec3 c = texture(tex, vTexCoord).rgb;
    vec3 b = max(c - vec3(threshold), vec3(0.0));
    b /= max(1.0 - threshold, 1e-4);
    outputColor = vec4(b, 1.0);
}

