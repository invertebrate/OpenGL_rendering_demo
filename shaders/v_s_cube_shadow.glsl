#version 410
layout(location = 0) in vec3 attr_pos;
uniform mat4 world;
void main() {
  gl_Position = world * vec4(attr_pos, 1.0);
}