#version 120

// implement your vertex shader here
varying vec3 Normal, LightDir;

void main() {
    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;

    Normal = gl_NormalMatrix * gl_Normal;
    LightDir = -(gl_ModelViewMatrix * gl_Vertex).xyz;
}
