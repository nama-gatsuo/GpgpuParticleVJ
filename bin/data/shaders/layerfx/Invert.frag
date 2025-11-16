#version 420

layout (binding = 0) uniform sampler2DRect tex0;

uniform int isBW;

in vec4 vColor;
in vec2 vTexCoord;
out vec4 outputColor;

void main() {

    vec4 col = texture(tex0, vTexCoord);

    if (isBW == 1) {
        col.rgb = vec3(dot(vec3(1.0) - col.rgb, vec3(0.299, 0.587, 0.114)));
    } else {
        col.rgb = vec3(1.) - col.rgb;
    }
    outputColor = col;
}
