#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in float texID;
layout (location = 3) in mat4 transformationMatrix;

out vec2 tc;
out float texIndex;

void main() {
	tc = texCoord;
	texIndex = texID;
	gl_Position = transformationMatrix * vec4(pos, 1.0);
}
