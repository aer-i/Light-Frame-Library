// glslangValidator -V -x -o text.vert.u32 text.shader.vert
#version 460

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec4 inColor;
layout(location = 2) in vec2 inUV;
layout(location = 3) in int  inTextureIndex;

layout(location = 0) out vec4 outColor;
layout(location = 1) out vec2 outUv;
layout(location = 2) flat out int outTextureIndex;

void main()
{
	outColor = inColor;
	outUv = inUV;
	outTextureIndex = inTextureIndex;
	gl_Position = vec4(inPosition * 2.0 - 1.0, 0.0, 1.0);
}