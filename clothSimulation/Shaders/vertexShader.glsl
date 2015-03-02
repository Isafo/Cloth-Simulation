# version 450

attribute vec3 normals;
attribute vec3 coord3d;
attribute vec3 v_color;
varying vec3 f_color; // for sending color from vertexshader to fragmentshader
varying vec3 vertPos;
uniform mat4 mv;
uniform mat4 p;

out vec3 interpolatedNormal;


void main(void) {

	interpolatedNormal = mat3(mv) * normals;
	gl_Position = mv * p * vec4(coord3d, 1.0);
	f_color = v_color;

	vec4 vertPos4 = p * vec4(coord3d, 1.0);
	vertPos = vec3(vertPos4) / vertPos4.w;
}