#version 310 es

#extension GL_OES_EGL_image_external : require

precision mediump float;

out vec4 oColor;

uniform float showAlpha;

uniform samplerExternalOES videoTex;

in vec2 UV;

void main() {
    vec4 tmp =texture( videoTex,UV );
    tmp.a = showAlpha;
    oColor =  tmp;
}
