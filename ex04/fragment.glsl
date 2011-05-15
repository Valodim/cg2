varying vec3 normal, lightDirection, eyeDirection;
uniform float innerSpotAngle, outerSpotAngle;

void main() {

	vec3 lightDirectionNorm = normalize(lightDirection);
	vec3 eyeDirectionNorm = normalize(lightDirection);

	float angle = acos(dot(lightDirectionNorm, eyeDirectionNorm));

	if(angle <= innerSpotAngle) {
		gl_FragColor.rgb = vec3(1.0, 1.0, 1.0);
	}
	else if(angle <= outerSpotAngle) {
		gl_FragColor.rgb = vec3(1.0, 1.0, 1.0) * (1 - ((angle - innerSpotAngle) / (outerSpotAngle - innerSpotAngle)));
	}
	else {
		gl_FragColor.rgb = vec3(0.2, 0.2, 0.2);
	}
}
