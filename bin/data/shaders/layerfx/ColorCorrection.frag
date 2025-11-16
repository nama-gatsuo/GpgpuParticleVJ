#version 400

in vec2 vTexCoord;
uniform sampler2DRect tex;

uniform float contrast;
uniform float saturation;
uniform float brightness;
uniform float gamma;

uniform float gain;
uniform vec4 multColor;

out vec4 outputColor;
/*
** Contrast, saturation, brightness
** Code of this function is from TGM's shader pack
** http://irrlicht.sourceforge.net/phpBB2/viewtopic.php?t=21057
*/

vec3 ContrastSaturationBrightness(vec3 color, float brt, float sat, float con) {
	const float AvgLumR = 0.5;
	const float AvgLumG = 0.5;
	const float AvgLumB = 0.5;

	const vec3 LumCoeff = vec3(0.2125, 0.7154, 0.0721);

	vec3 AvgLumin  = vec3(AvgLumR, AvgLumG, AvgLumB);
	vec3 brtColor  = color * brt;
	vec3 intensity = vec3(dot(brtColor, LumCoeff));
	vec3 satColor  = mix(intensity, brtColor, sat);
	vec3 conColor  = mix(AvgLumin, satColor, con);

	return conColor;
}

void main(){
    vec3 pixel = texture(tex, vTexCoord).rgb;
	vec3 color = ContrastSaturationBrightness(pixel, brightness, saturation, contrast );
    color = pow(color, vec3(gamma));

	color *= multColor.rgb;
	color *= gain;

    outputColor = vec4(color, 1.);
}
