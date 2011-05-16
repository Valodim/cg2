varying vec3 normal, eyeDirection;
uniform float innerSpotAngle, outerSpotAngle;

void main() {
	// Normalize vectors:
	vec3 eyeDirectionNorm = normalize(eyeDirection);

	// Calculate angle between viewing direction and vector from vertex to eye:
	float angle = acos(dot(vec3(0, 0, 1), eyeDirectionNorm));
	// Within innerSpotAngle apply full lighting:
	if(angle <= innerSpotAngle) {
		gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
	}
	// Linearly degrading lighting between innerSpotAngle and outerSpotAngle:
	else if(angle <= outerSpotAngle) {
		gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0) * (1.0 - ((angle - innerSpotAngle) / (outerSpotAngle - innerSpotAngle)));
	}
	// No ligthing otherwise:
	else {
		gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
	}
	// Apply ambient lighting:
	gl_FragColor += vec4(0.2, 0.2, 0.2, 1.0);
	// Degrade lighting with diverging angle between normal and eyeDirection:
	gl_FragColor *= dot(normal, eyeDirectionNorm);
}
