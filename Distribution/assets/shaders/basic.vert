#version 430

in vec3 position;

uniform mat4 worldMatrix;
uniform mat4 modelMatrix;

void main()
{
	gl_Position = worldMatrix * modelMatrix * vec4(position, 1);
}
