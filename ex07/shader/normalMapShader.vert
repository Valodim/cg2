#version 120

// XXX: read in your tangent, bitangent and normal values here
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

void main() {

  // stupid vec4..
  vec4 pos = vec4(att_position, 1.0);

  // viewing angle and lightdir, as usual
  viewing_angle = -(gl_ModelViewMatrix * pos).xyz;
  lightdir = (gl_ModelViewMatrix * (vec4(1.0) - pos)).xyz;

  // prepare normal, tangent and bitangent in world space
  vec3 normal = gl_NormalMatrix * att_normal;
  vec3 tangent = gl_NormalMatrix * att_tangent;
  vec3 bitangent = gl_NormalMatrix * att_bitangent;

  // Mt == M^-1 in this case
  mat4 Mt = mat4( tangent.x, bitangent.x, normal.x, 0,
                  tangent.y, bitangent.y, normal.y, 0,
                  tangent.z, bitangent.z, normal.z, 0,
                  0,         0,           0,        1);

  viewing_angle = (Mt * vec4(viewing_angle, 1.0)).xyz;

  gl_TexCoord[0].st = att_texcoord;
  gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * pos;

}
