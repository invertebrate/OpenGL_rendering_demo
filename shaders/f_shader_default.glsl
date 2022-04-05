#version 410
in vec2 texCoord;
out vec4 FragColor;
uniform sampler2D shadowmap;
void main() {
  FragColor = texture(shadowmap, texCoord);
}