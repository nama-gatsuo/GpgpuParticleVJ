#version 420

layout (binding = 0) uniform sampler2DRect tex;
uniform float amount;
in vec2 vTexCoord;
out vec4 outputColor;

const float aspect = 2.0/3.0;

float random (vec2 st) { 
    return fract(sin(dot(st.xy, vec2(12.9898,78.233)))*43758.5453123);
}

void main() {

    vec2 res = textureSize(tex);

    vec2 vUv = vTexCoord.xy / res.xy;
    vec2 r = vec2(random(vUv.xy), random(vUv.yx + 111.111));
    r -= 0.5;
    r *= 2.0;

    float dof0 = 0.01; // sqrt(2.0);
    float dof1 = 0.8;

    vec2 st = (vUv - 0.5) * 2.0;
    st.x *= aspect;

    float strength = smoothstep(dof0, dof1, distance(st, vec2(0, 0)));
    float dofRad = 0.04 * amount;

    vec4 original = texture(tex, (vUv + r * dofRad * strength) * res.xy);
    
    outputColor = vec4(original);

}