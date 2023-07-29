// glslangValidator -V -x -o default.vert.u32 default.shader.vert

#version 460

layout(location = 0) out struct
{
	vec3 color;
} Out;

layout(location = 0) in vec2 inPosition;

layout(push_constant) uniform CameraMatrix
{
	mat4 projView;
} camera;

void main()
{
    Out.color = vec3(1.0, 1.0, 1.0);
	gl_Position = camera.projView * vec4(inPosition, 0.0, 1.0);
}