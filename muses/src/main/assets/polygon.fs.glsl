#version 310 es

#extension GL_OES_EGL_image_external_essl3 : require

precision mediump float;

out vec4 oColor;

uniform samplerExternalOES videoTex;

in vec2 UV;

void main() {
    oColor = texture( videoTex,UV );
}
