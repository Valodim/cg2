varying vec3 normal, lightDirection, eyeDirection;

void main() {
	gl_Position = ftransform();

	normal = gl_NormalMatrix * gl_Normal;
	vec3 pos = (gl_ModelViewMatrix * gl_Vertex).xyz;
	lightDirection = gl_LightSource[0].position.xyz - pos;
	eyeDirection = -pos;
}
