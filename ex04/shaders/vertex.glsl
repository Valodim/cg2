varying vec3 normal, eyeDirection;

void main() {
	gl_Position = (gl_ProjectionMatrix * gl_ModelViewMatrix) * gl_Vertex;

	normal = gl_NormalMatrix * gl_Normal;
	vec4 pos = gl_ModelViewMatrix * gl_Vertex;
	eyeDirection = -(pos.xyz);
}
