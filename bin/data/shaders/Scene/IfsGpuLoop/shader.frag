#version 400

in vec4 vColor;
in float vDepth;
out vec4 outputColor;

void main() {

    vec2 p = gl_PointCoord.xy * 2.0 - 1.0;
    float r2 = dot(p, p);
    float body = smoothstep(1.0, 0.0, r2);
    float core = smoothstep(0.18, 0.0, r2);
    float depthFade = smoothstep(-15.0, 0.0, vDepth);

    float a = (body * 0.65 + core * 0.55) * vColor.a;
    outputColor = vec4(vColor.rgb, a);
}
