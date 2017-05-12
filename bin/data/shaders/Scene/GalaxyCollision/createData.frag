#version 410
#pragma include "../../Common/rand.glslinc.frag"

smooth in vec2 vTexCoord;

uniform vec2 res;

uniform float GM;
uniform float massRatio;

uniform vec3 p0;
uniform vec3 p1;
uniform vec3 v0;
uniform vec3 v1;

uniform float bulgeRad[2];
uniform float diskRad[2];
uniform float angleX[2];
uniform float angleY[2];

layout (location = 0) out vec4 outputColor1;
layout (location = 1) out vec4 outputColor2;

void rotateX(inout vec3 pos, float rad) {
    mat3 mat = mat3(
                    1.0,  0.0,      0.0,
                    0.0,  cos(rad), -sin(rad),
                    0.0,  sin(rad), cos(rad)
                    );
    
    pos = mat * pos;
}

void rotateY(inout vec3 pos, float rad) {
    mat3 mat = mat3(
                    cos(rad),  0.0, sin(rad),
                    0.0,       1.0, 0.0,
                    -sin(rad), 0.0, cos(rad)
                    );
    
    pos = mat * pos;
}

void main(){
    
    vec3 pos, vel;
    vec3 vAxis, vTan;
    
    float w = vTexCoord.x;
    float h = vTexCoord.y;
    
    vec2 co = vTexCoord;
    
    float coin = rand(co * vec2(1.01, 1.56));
    
    if (vTexCoord.y < 128.0) {
        
        if (coin < 0.7) {
            pos = getRandomVector(co * vec2(0.5, 0.99)) * diskRad[0];
            pos.y *= 0.05 * cos(PI * (dot(pos.xy, pos.xy) / (2.0 * diskRad[0])));
        } else if (coin < 0.9) {
            pos = getRandomXYZ(co * vec2(0.5, 0.99)) * bulgeRad[0];
            pos.y *= 0.2 * cos(PI * (dot(pos.xy, pos.xy) / (2.0 * bulgeRad[0])));
        } else {
            pos = getRandomVector(co * vec2(0.5, 0.99)) * 1.2;
        }
        
        if (abs(pos.y) < diskRad[0] * 0.1 && coin < 0.6) {
            vAxis = vec3(0.0, 1.0, 0.0);
        } else {
            vAxis = vec3(0.0, srand(co * 0.8), 1.0);
        }
        
        vTan = normalize(cross(vAxis, vec3(pos.x, 0.0, pos.z)));
        vel = vTan * sqrt(GM * massRatio);
        
        rotateX(pos, angleX[0]);
        rotateY(pos, angleY[0]);
        
        pos += p0;
        
        rotateX(vel, angleX[0]);
        rotateY(vel, angleY[0]);
        
        vel += v0;
        
    } else {
        
        if (coin < 0.7) {
            pos = getRandomVector(co * vec2(0.5, 0.99)) * diskRad[1];
            pos.y *= 0.05 * cos(PI * (dot(pos.xy, pos.xy) / (2.0 * diskRad[1])));
        } else if (coin < 0.9) {
            pos = getRandomXYZ(co * vec2(0.5, 0.99)) * bulgeRad[1];
            pos.y *= 0.2 * cos(PI * (dot(pos.xy, pos.xy) / (2.0 * bulgeRad[1])));
        } else {
            pos = getRandomVector(co * vec2(0.5, 0.99)) * 1.2;
        }
        
        if (abs(pos.y) < diskRad[0] * 0.1 && coin < 0.6) {
            vAxis = vec3(0.0, -1.0, 0.0);
        } else {
            vAxis = vec3(0.0, srand(co * 0.8), 1.0);
        }
        
        vTan = normalize(cross(vAxis, vec3(pos.x, 0.0, pos.z)));
        vel = vTan * sqrt(GM * massRatio);
        
        rotateX(pos, angleX[1]);
        rotateY(pos, angleY[1]);
        
        pos += p1;
        
        rotateX(vel, angleX[1]);
        rotateY(vel, angleY[1]);
        
        vel += v1;
    }
    
    outputColor1 = vec4(pos, 1.0); //position
    outputColor2 = vec4(vel, 1.0); //velocity
}
