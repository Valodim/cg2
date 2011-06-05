#version 120

// TODO: load your diffuse texture and the normal map
//       extract the normal for the current pixel
//       compute lighting using the extracted normal and the pixel-to-eye vector in tangent space

uniform sampler2D tex_diffuse;
uniform sampler2D tex_normal;

// in tangent space!!
varying vec3 viewing_angle;
varying vec3 lightdir;

void main() {

  // get normal from texture
  vec3 texnormal = texture2D(tex_normal, gl_TexCoord[0].st).rgb;
  texnormal.x = (texnormal.x -0.5)*2.0;
  texnormal.y = (texnormal.y -0.5)*2.0;
  texnormal.z = (texnormal.z -0.5)*2.0;

  // normalizin'
  vec3 V = normalize(viewing_angle);
  vec3 N = normalize(texnormal);
  vec3 L = normalize(lightdir);

  // terms with dot products in them
  float lambertian_term = dot(V, N);
  float light_strength = max(0, dot(L, N));

  // diffuse color from texture
  vec4 diffuse = texture2D(tex_diffuse, gl_TexCoord[0].st);

  // add them all up
  gl_FragColor = diffuse * light_strength * lambertian_term;

}
