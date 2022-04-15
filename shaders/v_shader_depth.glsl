#version 410
layout(location = 0) in vec3 aPos;
uniform mat4 screen;
void main() {
  gl_Position = screen * vec4(aPos, 1.0);
}