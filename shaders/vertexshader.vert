#version 330
in vec3 posAttr;
in vec3 normalAttr;
in vec3 colAttr;
out vec3 col;
uniform mat4 matrix;

void main()
{
   col = colAttr;
   gl_Position = matrix*vec4(posAttr,1);
}
