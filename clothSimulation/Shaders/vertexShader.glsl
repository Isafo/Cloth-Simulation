varying vec4 diffuse, ambientGlobal, ambient;
varying vec3 normal, lightDir, halfVec;
varying float dist;

void main() {

	vec4 ecPos;
	vec3 aux;

	normal = normalize(gl_NormalMatrix * gl_Normal);

	ecPos = gl_ModelViewMatrix * gl_Vertex;
	aux = vec3(gl_LightSource[0].position-ecPos);
	lightDir = normalize(aux);
	dist = length(aux);

	halfVec = normalize(gl_LightSource[0].halfVector.xyz);

	// calculate diffuse term
	diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;

	// calculate ambient terms
	ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
	ambientGlobal = gl_LightModel.ambient * gl_FrontMaterial.ambient;

	gl_Position = ftransform();
}