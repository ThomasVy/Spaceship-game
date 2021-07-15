#version 330 core
out vec4 color;

in vec2 tc;
in float texIndex;

uniform sampler2D samplers[3];

void main() {
	int texID = int(texIndex);
	vec4 d = texture(samplers[texID], tc);
	if(d.a < 0.01)
        discard; // If the texture is transparent, don't draw the fragment
	color = d;
} 
