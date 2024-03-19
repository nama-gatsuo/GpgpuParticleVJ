#version 410

in float vDepth;
in vec4 vColor;
out vec4 outputColor;

void main() {
    
    vec3 n;
    n.xy = gl_PointCoord.xy * 2.0 - 1.0;
    n.z = 1.0 - dot(n.xy, n.xy);

    float pAlpha = smoothstep(0.0, 1.0, n.z);
    pAlpha *= smoothstep(1.0, 0.0, vDepth);
    outputColor = vec4(vColor.rgb, pAlpha);

}
