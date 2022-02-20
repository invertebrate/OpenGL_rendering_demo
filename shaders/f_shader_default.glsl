#version 410
in vec2 texCoord;
out vec4 FragColor;
uniform sampler2D s;
void main() {
  FragColor = texture(s, texCoord);
}