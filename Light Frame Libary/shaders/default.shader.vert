// glslangValidator -V -x -o default.vert.u32 default.shader.vert

#version 460

layout(location = 0) out struct
{
	vec4 color;
} Out;

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec4 inColor;

layout(push_constant) uniform CameraMatrix
{
	mat4 projView;
} camera;

void main()
{
    Out.color = inColor;
	gl_Position = camera.projView * vec4(inPosition, 0.0, 1.0);
}