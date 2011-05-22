#version 120

uniform sampler2D tex;

// implement your fragment shader here

void main(){
    // gl_FragColor = texture2D(tex, gl_TexCoord[0].st);
    gl_FragColor = vec4(1.0f, 0.0f, 0.0f, 0.0f);
}
