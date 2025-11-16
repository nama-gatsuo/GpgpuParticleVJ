#version 420

layout (binding = 0) uniform sampler2DRect tex;

uniform float splitLength;
in vec2 vTexCoord;
out vec4 outputColor;

void main (void) {

    vec4 col = vec4(1.);
    col.r = texture(tex, vTexCoord + vec2(-35.) * splitLength).r;
    col.g = texture(tex, vTexCoord + vec2( -10.5, 0) * splitLength).g;
    col.b = texture(tex, vTexCoord + vec2(35., 45) * splitLength).b;

    // float grey = dot(col.rgb, vec3(1./3.));
    // float band = 8;
    // float f = mod(vTexCoord.x + vTexCoord.y, band);
    // float line = smoothstep(band - 1, band, f) * max(0.6 - grey , 0);

    //col.rgb -= line;
    outputColor = col;
}
