#version 410
layout(location = 0) in vec3 attr_pos;
layout(location = 1) in vec2 attr_tex;
layout(location = 2) in vec3 attr_nor;
uniform mat4 scale;
uniform mat4 rotation;
uniform mat4 translation;
uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 screen;
out vec2 tex_coord;
void main() {
  gl_Position = screen * vec4(attr_pos, 1.0);
  tex_coord = attr_tex;
}