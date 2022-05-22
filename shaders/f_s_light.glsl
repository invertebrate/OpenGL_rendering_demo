#version 410
out vec4 FragColor;
uniform vec3 light_color;
void main() {
  FragColor.xyz = light_color;
}