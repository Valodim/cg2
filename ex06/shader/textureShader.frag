#version 120

uniform sampler2D clouds;
uniform sampler2D clouds_alpha;
uniform sampler2D earth_night;
uniform sampler2D earth_day;

varying vec3 viewing_angle;
varying vec3 lightdir;
varying vec3 normal;

void main() {
  vec3 V = normalize(viewing_angle);
  vec3 L = normalize(lightdir);
  vec3 N = normalize(normal);

  float light_strength = max(0, dot(L, N));
  float lambertian_term = dot(V, N);

  float cloud_alpha = texture2D(clouds_alpha, gl_TexCoord[0].st).r;

  vec4 earth_ground = texture2D(earth_day, gl_TexCoord[0].st);
  vec4 cloud_layer = texture2D(clouds, gl_TexCoord[0].st);
  vec4 diffuse = cloud_alpha * cloud_layer + (1 - cloud_alpha) * earth_ground;
  vec4 ambient = diffuse * 0.2;

  vec4 emissive = texture2D(earth_night, gl_TexCoord[0].st) * pow((1 - light_strength), 2) * (1 - cloud_alpha);

  gl_FragColor = (diffuse * light_strength + ambient) * lambertian_term + emissive;
}
