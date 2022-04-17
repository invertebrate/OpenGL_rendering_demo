#version 410

in GS_OUT {
  vec2 tex_coord;
  vec3 normal;
  vec4 vtx_pos;
}
fs_in;

out vec4 FragColor;

uniform mat4 screen;
uniform mat4 world;

void main() {
  FragColor = vec4(1.0, 0.0, 1.0, 1.0);
}