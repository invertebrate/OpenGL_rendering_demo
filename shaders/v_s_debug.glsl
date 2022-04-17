#version 410
layout(location = 0) in vec3 attr_pos;
layout(location = 1) in vec2 attr_tex;
layout(location = 2) in vec3 attr_nor;

out VS_OUT {
  vec2 tex_coord;
  vec3 normal;
  vec4 pos;
}
vs_out;

uniform mat4 screen;
uniform mat4 world;

void main() {
  gl_Position = screen * vec4(attr_pos, 1.0);
  vs_out.tex_coord = attr_tex;
  vs_out.normal = attr_nor;
  vs_out.pos = vec4(attr_pos, 1.0);
}