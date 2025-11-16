#version 400
in vec2 vTexCoord;
uniform sampler2DRect tex;

uniform float freq;
uniform float time;
uniform float additiveFlicker;

out vec4 outputColor;

float rand2d(vec2 co) {
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

float rand(float n) {
    return fract(sin(n) * 43758.5453123);
}

float noise(float p) {
	float fl = floor(p);
  	float fc = fract(p);
	return mix(rand(fl), rand(fl + 1.0), fc);
}

float map(float val, float amin, float amax, float bmin, float bmax) {
    float n = (val - amin) / (amax-amin);
    float m = bmin + n * (bmax-bmin);
    return m;
}

float snoise(float p){
    return map(noise(p),0.0,1.0,-1.0,1.0);
}

float threshold(float val,float cut){
    float v = clamp(abs(val)-cut,0.0,1.0);
    v = sign(val) * v;
    float scale = 1.0 / (1.0 - cut);
    return v * scale;
}



vec2 uv_curve(vec2 uv) {
	uv = (uv - 0.5) * 2.0;
	uv *= 1.2;
	uv.x *= 1.0 + pow((abs(uv.y) / 5.0), 2.0);
	uv.y *= 1.0 + pow((abs(uv.x) / 4.0), 2.0);
    uv /= 1.15;
	uv  = (uv / 2.0) + 0.5;
	return uv;
}

vec3 color(sampler2DRect t, vec2 uv){
    return texture(t, uv * textureSize(t)).rgb;
}

vec3 ghost(sampler2DRect t, vec2 uv){

    float n1 = threshold(snoise(time*10.),.85);
    float n2 = threshold(snoise(2000.0+time*10.),.85);
    float n3 = threshold(snoise(3000.0+time*10.),.85);

    vec2 or = vec2(0.,0.);
    vec2 og = vec2(0,0.);
    vec2 ob = vec2(0.,0);

    float os = .05;
    or += vec2(n1*os,0.);
    og += vec2(n2*os,0.);
    ob += vec2(0.,n3*os);

    float r = color(t, uv + or).r;
    float g = color(t, uv + og).g;
    float b = color(t, uv + ob).b;

    return vec3(r,g,b);

}

vec2 uv_ybug(vec2 uv){
    float n4 = clamp(noise(200.0+time*2.)*14.,0.,2.);
    uv.y += n4;
    uv.y = mod(uv.y,1.);
    return uv;
}

vec2 uv_hstrip(vec2 uv){
    float vnoise = snoise(time*6.);
    float hnoise = threshold(snoise(time*10.),.5);

    float line = (sin(uv.y*10.+vnoise)+1.)/2.;
    line = (clamp(line,.9,1.)-.9)*10.;

    uv.x += line * 0.03 * hnoise;
    uv.x = mod(uv.x,1.);
    return uv;
}



void main(){
    float t = float(int(time * freq));
    vec2 res = textureSize(tex);
    vec2 uv = vTexCoord / res;

    uv = uv_curve(uv);

    vec2 ouv = uv;

    float xn = threshold(snoise(time*10.),.7) * 0.05;
    float yn = threshold(snoise((500.0+time)*10.),.7) * 0.05;

    float r = rand2d(uv+(t+100.0)*.01);
    uv = uv + vec2(xn,yn) * r;

    uv = uv_ybug(uv);

    uv = uv_hstrip(uv);

    vec2 onePixel = vec2(0.0, 1.0) / res * 3.;

    vec3 color = ghost(tex, uv);

    //color = colorC;

    float scanA = (sin(uv.y*3.1415*res.y/2.7)+1.)/2.;
    float scanB = (sin(uv.y*3.1415*1.)+1.)/2.;
    color *= .75 + scanA * .25;
    //color *= .5 + scanC * .5;
    //color *= scanB;

    float blink = .96 + .04*(sin(time*100.)+1.)/2.;
    color *= blink;

    float vig = 44.0 * (ouv.x * (1.0-ouv.x) * ouv.y * (1.0-ouv.y));
	vig *= mix( 0.7, 1.0, rand(t + 0.5));
    color *= .6 + .4*vig;


    color *= 1.0 + rand2d(uv+t*.01) * 0.2;

    vec3 backColor = vec3(0);
    if (ouv.x < 0.0 || ouv.x > 1.0)
		color = backColor;
	if (ouv.y < 0.0 || ouv.y > 1.0)
		color = backColor;

    outputColor = vec4(color, 1.);
}
