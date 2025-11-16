#version 400

// Original shader by hornet
// https://www.shadertoy.com/view/XssGz8
uniform sampler2DRect tex;
uniform vec2 res;
uniform float barrelPower;

in vec2 vTexCoord;
out vec4 outputColor;

const int num_iter = 12;
const float reci_num_iter_f = 1.0 / float(num_iter);

vec2 barrelDistortion(vec2 coord, float amt) {
    vec2 cc = (coord - 0.5) * (1. - barrelPower * 0.3);
    float dist = dot(cc, cc);
    return coord + cc * dist * amt;
}

float sat(float t) {
    return clamp(t, 0.0, 1.0);
}

float linterp(float t) {
    return sat(1.0 - abs(2.0 * t - 1.0));
}

float remap(float t, float a, float b) {
    return sat((t - a) / (b - a));
}

vec3 spectrumOffset(float t) {
    float lo = step(t, 0.5);
    float hi = 1.0 - lo;
    float w = linterp(remap(t, 1.0/6.0, 5.0/6.0));
    vec3 ret = vec3(lo, 1.0, hi) * vec3(1.0 - w, w, 1.0 - w);
    return pow(ret, vec3(1.0/2.2));
}

vec2 fitting(vec2 uv, vec2 offset, vec2 scale) {
    return offset + uv * scale;
}

void main() {
    vec2 uv = vTexCoord / res;

    vec3 sumcol = vec3(0.);
    vec3 sumw = vec3(0.);

    // scaling
    float maxT = reci_num_iter_f * (num_iter - 1.0);
    vec2 minUv = barrelDistortion(vec2(0.0), barrelPower * maxT);
    vec2 maxUv = barrelDistortion(vec2(1.0), barrelPower * maxT);
    vec2 scale = vec2(1.0) / (maxUv - minUv);
    vec2 offset = - minUv * scale;

    for (int i = 0; i < num_iter; ++i) {
        float t = float(i) * reci_num_iter_f;
        vec3 w = spectrumOffset(t);
        sumw += w;

        vec2 nUv = barrelDistortion(uv, barrelPower * t);
        nUv = fitting(nUv, offset, scale);
        nUv *= res;
        
        sumcol += w * texture(tex, nUv).rgb;
    }

    outputColor = vec4(sumcol / sumw, 1.0);

}
