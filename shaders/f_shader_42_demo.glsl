#version 460
in vec2 texCoord;
in float blend;
in flat vec3 pos;
out vec4 FragColor;
uniform sampler2D s;
void main() {
  FragColor = (1 - blend) * texture(s, texCoord) + blend * vec4(pos / 2, 1.0);
}