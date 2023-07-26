// glslangValidator -V -x -o default.vert.u32 default.shader.vert

#version 460

layout(location = 0) out struct
{
	vec3 color;
} Out;

layout(location = 0) in vec2 inPosition;

void main()
{
    Out.color = vec3(1.0, 1.0, 1.0);
	gl_Position = vec4(inPosition, 0.0, 1.0);
}