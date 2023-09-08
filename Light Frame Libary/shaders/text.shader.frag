// glslangValidator -V -x -o text.frag.u32 text.shader.frag
#version 460

#extension GL_EXT_nonuniform_qualifier : require

layout(binding = 0) uniform sampler2D textures[];

layout(location = 0) in vec4 inColor;
layout(location = 1) in vec2 inUv;
layout(location = 2) flat in int inTextureIndex;

layout(location = 0) out vec4 outColor;

void main()
{
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(textures[nonuniformEXT(inTextureIndex)], inUv).r);

	outColor = inColor * sampled;
}