#version 330 core
in vec2 texcoord;

out vec4 FragColor;

// uniform float nice;
uniform sampler2D tex0;

void main()
{
    FragColor = texture(tex0, texcoord);
}