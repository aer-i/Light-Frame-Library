// glslangValidator -V -x -o default.frag.u32 default.shader.frag

#version 460

layout(location = 0) out vec4 outColor;

layout(location = 0) in struct
{
	vec3 color;
} In;

void main()
{
	outColor = vec4(In.color, 1.0);
}