#version 330 core //Version Number

//the position variable has attribute position 0
//Accesses the vertex position and assigns it to aPos
layout(location = 0) in vec3 aPos; 


out vec3 texCoord;


//Creates a Transform variable
//We'll assign the Transformation matrix here


uniform mat4 projection;

uniform mat4 view;

void main()
{
    vec4 pos = projection  * view * vec4(aPos, 1.0);    
    gl_Position = vec4(pos.x, pos.y, pos.w, pos.w); 
    texCoord = aPos;

    
}
