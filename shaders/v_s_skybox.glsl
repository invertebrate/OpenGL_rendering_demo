#version 410
layout(location = 0) in vec3 attr_pos;
out vec3 tex_coords;
uniform mat4 screen;
void main() {
  tex_coords = attr_pos;
  gl_Position = screen * vec4(attr_pos, 1.0);
}