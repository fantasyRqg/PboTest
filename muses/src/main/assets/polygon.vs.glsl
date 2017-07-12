#version 310 es

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 vUV;

uniform mat4 mvpMat;

out vec2 UV;

void main() {

    gl_Position = mvpMat * vec4(aPosition,1.0);

    UV = vUV;
}
