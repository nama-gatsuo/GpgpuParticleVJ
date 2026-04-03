#version 420

layout (binding = 0) uniform sampler2DRect tex;
uniform sampler2DRect blurTex;
uniform float strength;
in vec2 vTexCoord;
out vec4 outputColor;

void main() {
    vec3 base = texture(tex, vTexCoord).rgb;
    vec3 bloom = texture(blurTex, vTexCoord).rgb;
    outputColor = vec4(base + bloom * strength, 1.0);
}

