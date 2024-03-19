#version 400
precision mediump float;

in float vDepth;
in vec4 vColor;
out vec4 outputColor;

void main() {
    
    float gamma = 1.0 / 2.2;
    
    vec3 n;
    n.xy = gl_PointCoord.xy * 2.0 - 1.0;
    n.z = 1.0 - dot(n.xy, n.xy);
    
    float pAlpha = smoothstep(0.0, 1.0, n.z);
    pAlpha *= smoothstep(-10.0, 0.0, vDepth);

    outputColor = vec4(vColor.rgb, pAlpha);
}
