//Pyramiden
#version 330 core
uniform mat4 Mv; //Modelview: two transformations
uniform mat4 P;  //Projection
uniform mat4 RMV;
uniform mat4 RMH;
layout ( location = 0) in vec3 Position ;
layout ( location = 1) in vec3 Normal ; //Color
layout ( location = 2) in vec2 Texture ;
out vec2 interpolatedTexture ;
out vec3 interpolatedNormal;

void main (){
     
//     vec3 L = normalize(mat3(Mv)*vec3(-1,1,0));
//     vec3 V = vec3(0,0,1);
//     vec3 N = normalize(Normal);
//     float D = 0.5*1*max(0.0,dot(L,N));
//     vec3 R = normalize(reflect(-L,N)); //2*(dot(L,N)*N)-L;
//     float Ispec = 0.5*pow(max(0.0,dot(V,R)),20.0);
     
     //interpolatedColor = vec3(D + Ispec);
     interpolatedNormal = Normal;
     interpolatedTexture = Texture;
     
     vec4 sist = RMV*RMH*vec4 ( Position*0.5 , 1.0);
     gl_Position = sist ;
}
