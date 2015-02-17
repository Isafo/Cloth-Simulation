#version 330 core
uniform mat4 Mv; //Modelview: two transformations
uniform mat4 P;  //Projection
uniform mat4 V; // View matrix
layout ( location = 0) in vec3 Position ;
layout ( location = 1) in vec3 Normal ; //Color
layout ( location = 2) in vec2 TextureUV ;
out vec2 interpolatedTexture ;
out vec3 interpolatedNormal;

void main (){

     interpolatedNormal = Normal;
     UV = TextureUV;
     
     gl_Position = P * v * Mv * vec4 ( Position , 1.0);
}
