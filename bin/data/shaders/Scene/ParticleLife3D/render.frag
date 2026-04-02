#version 410

smooth in vec4 vColor;
in float vDepth;
out vec4 outputColor;

void main() {

    vec3 n;
    n.xy = gl_PointCoord.xy * 2.0 - 1.0;
    n.z = 1.0 - dot(n.xy, n.xy);

    float pAlpha = smoothstep(0.0, 1.0, n.z * 0.9);
    // pAlpha *= smoothstep(-14.0, 0.0, vDepth);

    outputColor = vec4(vColor.rgb, pAlpha * vColor.a * 1.35);
}
