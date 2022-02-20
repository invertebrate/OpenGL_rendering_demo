#version 410
in vec2 texCoord;
in float blend;
flat in vec3 col;
out vec4 FragColor;
uniform sampler2D s;
vec3 grey;
void main() {
  grey.x = ((col.x + col.y + col.z) / 3 + 1) / 2;
  grey.y = grey.x;
  grey.z = grey.x;
  FragColor = (1 - blend) * texture(s, texCoord) + blend * vec4(grey / 3, 1.0);
}