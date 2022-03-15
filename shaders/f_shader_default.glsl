#version 410
in vec2 texCoord;
out vec4 FragColor;
uniform sampler2D diffuse;
void main() {
  FragColor = texture(diffuse, texCoord);
}