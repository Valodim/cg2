#version 120

varying vec3 viewing_angle;
varying vec3 lightdir;
varying vec3 normal;

void main() {
  viewing_angle = -(gl_ModelViewMatrix * gl_Vertex).xyz;
  lightdir = (gl_ModelViewMatrix * (vec4(1.0) - gl_Vertex)).xyz;
  normal = gl_NormalMatrix * gl_Normal;
  gl_TexCoord[0] = gl_MultiTexCoord0;
  gl_Position = ftransform();
}
