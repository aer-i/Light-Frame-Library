// glslangValidator -V -x -o default.frag.u32 default.shader.frag

#version 460

#extension GL_EXT_nonuniform_qualifier : require

layout(binding = 0) uniform sampler2D textures[];

layout(location = 0) in vec4 inColor;
layout(location = 1) in vec2 inUv;
layout(location = 2) flat in int inTextureIndex;

layout(location = 0) out vec4 outColor;

void main()
{
	outColor = vec4(inColor.xyzw * texture(textures[nonuniformEXT(inTextureIndex)], inUv).rgba);
}