#version 120

// TODO: read in your tangent, bitangent and normal values here
//       compute M' as matrix mapping world space to tangent space
//       transform the pixel-to-eye vector into tangent space and pass it to the fragment shader

attribute vec3 att_position;
attribute vec3 att_normal;
attribute vec2 att_texcoord;
attribute vec3 att_tangent;
attribute vec3 att_bitangent;

// in tangent space!!
varying vec3 viewing_angle;
varying vec3 lightdir;

varying vec3 normal;
varying vec3 tangent;
varying vec3 bitangent;

void main() {

  vec4 pos = vec4(att_position, 1.0);

  viewing_angle = -(gl_ModelViewMatrix * pos).xyz;
  lightdir = (gl_ModelViewMatrix * (vec4(1.0) - pos)).xyz;

  normal = gl_NormalMatrix * att_normal;
  tangent = (gl_ModelViewMatrix * vec4(att_tangent, 1.0)).xyz;
  bitangent = (gl_ModelViewMatrix * vec4(att_bitangent, 1.0)).xyz;

  mat4 Mt = mat4( tangent.x, bitangent.y, normal.z, 0,
                  tangent.x, bitangent.y, normal.z, 0,
                  tangent.x, bitangent.y, normal.z, 0,
                  0,         0,           0,        1);

  viewing_angle = (Mt * vec4(viewing_angle, 1.0)).xyz;

  gl_TexCoord[0].st = att_texcoord;
  gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * pos;

}
