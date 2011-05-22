#version 120

varying vec3 vert_light_dir;
varying vec3 vert_norm_dir;

uniform float uni_outerSpotAngle;
uniform float uni_innerSpotAngle;

void main () {

    // normalize everything necessary //
    vec3 N = normalize(vert_norm_dir);
    vec3 L = normalize(vert_light_dir);
    vec3 X = vec3(0, 0, 1);

    // diffuse component //
    float angle = acos(dot(X, L));
    if(angle < uni_innerSpotAngle)
        gl_FragColor = vec4(0.5, 0.5, 0.0, 1.0);
    else if(angle < uni_outerSpotAngle)
        gl_FragColor = vec4(0.0, 0.5, 0.5, 1.0) * (1 - (angle - uni_innerSpotAngle) / (uni_outerSpotAngle - uni_innerSpotAngle));
    else
        gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);

    gl_FragColor += vec4(0.2, 0.2, 0.2, 0.0);

    gl_FragColor *= dot(N, L);

}
