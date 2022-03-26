#version 410
layout(location = 0) in vec3 aPos;
out vec3 TexCoords;
uniform mat4 screen;
void main() {
  TexCoords = aPos;
  gl_Position = screen * vec4(aPos, 1.0);
}