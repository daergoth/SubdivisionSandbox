#version 330
in vec3 col;
out vec4 fragcol;

void main(void)
{
   fragcol = vec4(col,1);
}
