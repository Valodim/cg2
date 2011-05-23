#version 120

uniform sampler2D tex;
varying vec3 Normal, LightDir;

// implement your fragment shader here

void main(){
    gl_FragColor = texture2D(tex, gl_TexCoord[0].st);
    //gl_FragColor = vec4(1.0f, 0.0f, 0.0f, 0.0f);

    vec3 N = normalize(Normal);
    vec3 L = normalize(LightDir);

    float NdotL = max(0.0, dot(N, L));
    gl_FragColor = gl_FragColor * NdotL;
}
