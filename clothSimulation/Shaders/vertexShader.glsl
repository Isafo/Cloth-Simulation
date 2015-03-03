# version 430


attribute vec3 coord3d;
attribute vec3 v_color;
varying vec3 f_color; // for sending color from vertexshader to fragmentshader
uniform mat4 mvp; //  global transformation matrix

void main(void) {
	gl_Position = mvp * vec4(coord3d, 1.0);
	f_color = v_color; // send the color to the fragmentshader
}