#version 300 es

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aColor;

out vec4 vColor;

uniform mat4 mvpMat;

void main() {
    vColor = vec4(aColor,1.0);
    gl_Position = mvpMat * vec4(aPosition,1.0);
}
