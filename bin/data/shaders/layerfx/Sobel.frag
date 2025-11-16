#version 400

uniform sampler2DRect tex;
in vec2 vTexCoord;
out vec4 outputColor;

void main() {
    const vec2 offset[9] = vec2[](
        vec2(-1.0, -1.0), vec2( 0.0, -1.0), vec2( 1.0, -1.0),
        vec2(-1.0,  0.0), vec2( 0.0,  0.0), vec2( 1.0,  0.0),
        vec2(-1.0,  1.0), vec2( 0.0,  1.0), vec2( 1.0,  1.0)
    );

    const float hCoef[9] = float[](
        1.0,  0.0, -1.0,
        2.0,  0.0, -2.0,
        1.0,  0.0, -1.0
    );

    const float vCoef[9] = float[](
        1.0,  2.0,  1.0,
        0.0,  0.0,  0.0,
        -1.0, -2.0, -1.0
    );

    vec3 horizonColor = vec3(0.0);
    vec3 verticalColor = vec3(0.0);

    for (int i = 0; i < 9; i++) {
        vec3 c = texture(tex, vTexCoord + offset[i]).rgb;
        horizonColor += c * hCoef[i];
        verticalColor += c * vCoef[i];
    }

    outputColor.rgb = smoothstep(vec3(0), vec3(1), vec3(sqrt(horizonColor * horizonColor + verticalColor * verticalColor)));
    outputColor.a = 1.0;
}
