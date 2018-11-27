#version 330
in vec3 col;
out vec4 fragcol;
uniform vec4 tint;

void main(void)
{
   fragcol = vec4(col,1)*tint;
}
