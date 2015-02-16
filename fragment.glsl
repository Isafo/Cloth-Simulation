//Pyramiden
#version 330 core
in vec2 interpolatedTexture;
in vec3 interpolatedNormal;
uniform mat4 Mv;
uniform mat4 P;
uniform mat4 Ljus;
uniform mat4 RMV;
uniform mat4 RMH;
out vec4 color;
uniform sampler2D tex; //A uniform variable to identify the texture in
//vec2 st; //Interpolated texture coords, sent from vertex shader

void main (){

     vec3 L = normalize(mat3(Mv)*vec3(1,1,1));
     vec3 V = vec3(0,0,1);
     vec3 N = normalize(mat3(RMV*RMH)*interpolatedNormal);
     float D = 0.15 + 0.5*1*max(0.0,dot(L,N));
     vec3 R = normalize(reflect(-L,N)); //2*(dot(L,N)*N)-L;
     float Ispec = 0.5*pow(max(0.0,dot(V,R)),20.0);
     
     vec3 klar = vec3(D + Ispec)*vec3(1,1,1);
     
     vec4 light = vec4(klar,1.0);



       //color = vec4(interpolatedTexture, 0 ,1);
       color = texture(tex, interpolatedTexture)*light; //Use the texture to set the surface color

}
