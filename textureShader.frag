#version 330 core
in vec2 TexCoord;
in vec3 ourColor;

out vec4 color;

// Texture samplers
uniform sampler2D ourTexture;

void main()
{
	color = texture(ourTexture, TexCoord);
}