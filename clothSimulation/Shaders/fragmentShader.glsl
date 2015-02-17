#version 330 core

in vec2 UV; // the UV coordinates
in vec3 interpolatedNormal;
uniform mat4 Mv; //Modelview: two transformations
uniform mat4 P; //Projection
uniform mat4 V; // View matrix
out vec4 color;
uniform sampler2D tex; //A uniform variable to identify the texture in

void main (){

	float ks = 0.5;
	float kd = 0.5;
	float ka = 0.15;
	float ns = 20;

     vec3 L = normalize(mat3(Mv) * vec3(1, 1, 1));
     vec3 V = vec3(0, 0, 1);
     vec3 N = normalize(mat3(Mv) * interpolatedNormal);
     float Idiff = ka + kd * max(0.0, dot(L, N));
     vec3 R = normalize(reflect(-L, N)); // 2*(dot(L,N)*N)-L;
     float Ispec = ks*pow(max(0.0,dot(V,R)),ns);
     
     vec3 phong = vec3(Idiff + Ispec)*vec3(1, 1, 1);
     
     vec4 light = vec4(phong, 1.0);

     color = texture(tex, UV) * light; //Use the texture to set the surface color

}
