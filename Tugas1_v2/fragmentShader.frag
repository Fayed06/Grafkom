#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

// Texture samplers
uniform sampler2D ourTexture;

void main()
{
    FragColor = texture(ourTexture, TexCoord);
}