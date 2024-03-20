#version 400

uniform mat4 modelViewProjectionMatrix;
in vec4 position; // random val in -1 ~ 1
in vec4 color;

out vec4 vColor;

uniform float alpha;
uniform float params[6];
uniform float scale;

void main(){

    vec4 p = position;

    float x, y, z;

    for (int i = 0; i < 32; i++) {

        x = p.z * sin(params[0] * p.x) + cos(params[1] * p.y);
        y = p.x * sin(params[2] * p.y) + cos(params[3] * p.z);
        z = p.y * sin(params[4] * p.z) + cos(params[5] * p.x);

        p.x = x;
        p.y = y;
        p.z = z;
    }

    p.x = p.x * scale - scale / 2.0;
    p.y = p.y * scale - scale / 2.0;
    p.z = p.z * scale - scale / 2.0;

    gl_Position = modelViewProjectionMatrix * p;
    gl_PointSize = 500.0 / gl_Position.w;

    vColor = color * alpha;
}
