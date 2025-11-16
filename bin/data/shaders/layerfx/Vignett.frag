#version 400
uniform vec2 res;
uniform sampler2DRect tex;
uniform float fallOff;
in vec2 vTexCoord;
out vec4 outputColor;

void main() {
	vec2 uv = (vTexCoord / res - 0.5) * 2.;
    float rf = sqrt(dot(uv, uv)) * fallOff;
    float rf2_1 = rf * rf + 1.0;
    float e = 1.0 / (rf2_1 * rf2_1);

    vec3 src = texture(tex, vTexCoord).rgb;
	outputColor = vec4(src * e, 1.0);
}
