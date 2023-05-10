#version 410

uniform sampler2DRect tex0;
uniform float isNega;
in vec4 vColor;
in vec2 vTexCoord;
out vec4 outputColor;

void main() {
    
    vec4 col = texture(tex0, vTexCoord);
    col.rgb = pow(col.rgb, vec3(1.0/2.2));
    col.rgb = vec3(col.r + col.g + col.b) / 3.0;
    if (isNega > 0.5) {
        col.rgb = vec3(1.0) - col.rgb;
    }

    outputColor = col;
}
